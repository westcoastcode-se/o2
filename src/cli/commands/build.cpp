//
// Part of the o2 Project, under the Apache License v2.0 with o2 Project Exceptions.
// See the LICENSE file in the project root for license terms
//

#include "build.h"
#include <sstream>
#include <iostream>
#include <sstream>
#include <filesystem>
#include <utility>
#include <fstream>

using namespace o2;

namespace
{
	static inline unsigned long long now()
	{
		return std::chrono::duration_cast<std::chrono::milliseconds>(
				std::chrono::system_clock::now().time_since_epoch()).count();
	}
}

build::build(config cfg)
		: _config(std::move(cfg)), _context(), _syntax_tree(_context), _pending_requests(),
		  _system_module(_config.lang_path, &_syntax_tree),
		  _main_module(), _aborted()
{
}

build::~build()
{
	assert(!_parse_responses.is_open());
	assert(!_parse_requests.is_open());
}

int build::execute()
{
	const auto start = now();

	// spawn all threads used for the actual processing
	for (int i = 0; i < _config.threads_count; ++i)
	{
		_threads.emplace_back([requests = &_parse_requests, responses = &_parse_responses]
		{
			// process requests as quickly as possible, as long as the request channel is open
			while (requests->is_open())
			{
				async_data* data;
				if (!requests->wait_pop(&data))
					continue;
				responses->put(build::parse(data));
			}
		});
	}

	// TODO: Parse module file in the config root path and figure out the module name form that
	const string_view module_name(STR("westcoastcode.se/hello_world"));
	// Initialize the module we are compiling and add it to the syntax tree
	_main_module = o2_new module(&_system_module, module_name, _config.path);
	_main_module->insert_into(&_syntax_tree);

	// Prepare a parser state used for the main application
	parser_state state(&_syntax_tree);

	bool success = true;
	try
	{
		// convert the "path" where the main source code is found in the module
		// into a module-relative path
		string app(_main_module->get_name());
		if (_config.path != ".")
		{
			app += STR("/");
			app += _config.path.generic_string();
		}

		parse_main_module_package(_main_module, app, &state);
		if (_config.verbose_level > 0)
			std::cout << "parsed '" << app << "' - done!" << std::endl;
	}
	catch (const o2::error& e)
	{
		// print errors, but continue evaluating all other source code
		// because it's tedious to having to fix one problem at a time
		e.print(std::cerr);
		success = false;
	}

	// do import everything found in the main source code
	auto imports = state.get_imports();
	for (auto i: imports)
	{
		const auto m = _main_module->find_module(i->get_import_statement());
		if (m != nullptr)
		{
			const auto sources = m->get_package_info(i->get_import_statement());
			try_import(new async_data(&_syntax_tree), i, m, sources);
		}
		else
		{
			std::cerr << "could not import '" << i->get_import_statement() << "' because module could not be found"
					  << std::endl;
			success = false;
		}
	}

	// no imports so we can already resolve all properties
	if (imports.empty())
	{
		const recursion_detector rd;
		for (auto p: _main_module->get_packages())
			p->process_phases();
	}

	// handle each parse request and import statement
	while (_parse_responses.is_open() && _pending_requests > 0)
	{
		async_data* data;
		if (!_parse_responses.wait_pop(&data))
			continue;
		_pending_requests--;

		// Did an error happen when we parsed the source code?
		if (!data->out.errors.empty())
		{
			// log the parse failure
			data->in.package_info->load_status = package_source_info::failed;
			for (const auto& e: data->out.errors)
				std::cerr << e << std::endl;
			success = false;
			// delete the data
			delete data;
			// try process more responses
			continue;
		}

		// Logging
		assert(data->out.package);
		if (_config.verbose_level > 0)
		{
			if (data->in.package_info->relative_path.empty())
				std::cout << "parsed '" << data->in.module->get_name() << "' - done!" << std::endl;
			else
				std::cout << "parsed '" << data->in.module->get_name() << "/" << data->in.package_info->relative_path
						  << "' - done!" << std::endl;
		}

		// the load completed successfully
		assert(data->in.package_info);
		data->in.package_info->load_status = package_source_info::successful;

		// add the loaded package to the module
		assert(data->in.module);
		const auto imported_module = data->in.module;
		const auto imported_package = data->out.package;
		imported_module->add_package(imported_package);

		// collect all imports to be parsed
		imports = data->out.state.get_imports();
		if (imports.empty())
		{
			// package is now imported
			imported_module->notify_package_imported(imported_package);
			try
			{
				// all imports are imported, so let's resolve this package's dependencies!
				imported_package->process_phases();
			}
			catch (const o2::error& e)
			{
				// print errors, but continue evaluating all other source code
				// because it's tedious to having to fix one problem at a time
				e.print(std::cerr);
				success = false;
			}
		}
		else
		{
			auto imports_left = imports.size();
			for (auto i: imports)
			{
				const auto m = imported_module->find_module(i->get_import_statement());
				if (m)
				{
					const auto sources = m->get_package_info(i->get_import_statement());
					switch (try_import(data, i, m, sources))
					{
					case package_source_info::loading:
						data = nullptr;
						break;
					case package_source_info::successful:
						imports_left--;
						break;
					default:
						break;
					}
				}
			}

			// package is now imported. This is done after, potentially, importing more packages to lessen the time
			// we have to wait and sleep
			imported_module->notify_package_imported(imported_package);

			// all imports are imported, so let's resolve this package
			if (imports_left == 0)
			{
				try
				{
					imported_package->process_phases();
				}
				catch (const o2::error& e)
				{
					// print errors, but continue evaluating all other source code
					// because it's tedious to having to fix one problem at a time
					e.print(std::cerr);
					success = false;
				}
			}
		}
		// delete any state that's pending
		delete data;
	}
	_parse_responses.close();
	_parse_requests.close();

	// wait for all threads to shut down
	std::for_each(_threads.begin(), _threads.end(), [](std::jthread& j) -> void
	{
		j.join();
	});

	if (success)
	{
		try
		{
			optimize(&_syntax_tree, 0);
		}
		catch (const o2::error& e)
		{
			// print errors, but continue evaluating all other source code
			// because it's tedious to having to fix one problem at a time
			e.print(std::cerr);
			success = false;
		}
	}

	const auto diff = now() - start;
	if (!success)
		return 1;

	switch (_config.output_type)
	{
	case build_config_output::json:
		return output_json();
		break;
	case build_config_output::binary:
		std::cerr << "binary output not added yet!" << std::endl;
		break;
	case build_config_output::debug:
		std::cout << "build ok - " << diff << " milliseconds" << std::endl;
		_syntax_tree.debug();
		break;
	}
	return 0;
}

package_source_info::status build::try_import(async_data* data, node_import* import_request, module* imported_module,
		package_source_info* package_info)
{
	// we only care about package sources that's not loaded
	if (package_info->load_status != package_source_info::not_loaded)
	{
		// the import is already loaded so let's notify the actual import and tell it that
		// it's already loaded
		if (package_info->load_status == package_source_info::successful)
			import_request->notify_imported();
		else
			imported_module->add_import_request(import_request);
		return package_info->load_status;
	}

	// initialize the async data
	if (data == nullptr)
		data = new async_data(&_syntax_tree);
	data->in.module = imported_module;
	package_info->load_status = package_source_info::loading;
	data->in.package_info = package_info;
	data->out.errors.clear();
	data->out.package = nullptr;

	// put the data to be processed by one of the worker threads
	_parse_requests.put(data);
	_pending_requests++;

	// put the import as being imported in the future
	imported_module->add_import_request(import_request);
	return package_info->load_status;
}

async_data* build::parse(async_data* data)
{
	try
	{
		// load the actual string source code
		data->in.module->load_package_sources(data->in.package_info);
		// parse the source code
		data->out.package = parse_package_sources(data->in.package_info->sources,
				data->in.package_info->name,
				&data->out.state);
	}
	catch (const error& e)
	{
		std::stringstream ss;
		e.print(ss);
		data->out.errors.emplace_back(std::move(ss.str()));
	}
	catch (const std::exception& e)
	{
		data->out.errors.emplace_back(e.what());
	}

	return data;
}

void build::abort()
{
	_aborted = true;
	_parse_requests.close();
	_parse_responses.close();
}

int build::output_json()
{
	if (_config.output_destination.empty())
	{
		stringstream s;
		{
			json j(&s);
			_syntax_tree.get_root_package()->write_json(j);
		}
		std::cout << s.str();
		return 0;
	}
	else
	{
		std::ofstream output_stream(std::filesystem::path(_config.output_destination), std::ios::trunc);
		if (output_stream.is_open())
		{
			{
				json j(&output_stream);
				_syntax_tree.get_root_package()->write_json(j);
			}
			output_stream.close();
			return 0;
		}
		else
		{
			std::cerr << "could not write to '" << _config.output_destination << "'" << std::endl;
			return 1;
		}
	}
}
