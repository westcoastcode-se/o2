//
// Part of the o2 Project, under the Apache License v2.0 with o2 Project Exceptions.
// See the LICENSE file in the project root for license terms
//

#include "build.h"
#include <sstream>
#include <iostream>
#include <sstream>
#include <filesystem>

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
		: _config(cfg), _import_statements_running(0)
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

	for (int i = 0; i < _config.threads_count; ++i)
	{
		_threads.emplace_back([requests = &_parse_requests, responses = &_parse_responses]
		{
			// process requests as quickly as possible, as long as the request channel is open
			while (requests->is_open())
			{
				async_data* data;
				if (!requests->try_pop(&data))
					continue;
				responses->put(build::parse(data));
			}
		});
	}

	string_view module_root_path = _config.path;
	if (module_root_path == ".")
		module_root_path = string_view();

	// TODO: Parse module file in the config root path and figure out the module name form that
	const string_view module_name("westcoastcode.se/hello_world");
	// Initialize the module we are compiling and add it to the syntax tree
	_main_module = o2_new module(module_name, module_root_path,
			new filesystem_module_source_codes(_config.path));
	_syntax_tree.get_root_package()->add_child(_main_module);

	// Prepare a parser state used for the main application
	parser_state state(&_syntax_tree);

	bool success = true;
	try
	{
		// convert the "path" where the main source code is found in the module
		// into a module-relative path
		string app(_main_module->get_name());
		if (module_root_path.length() > 0)
		{
			app += "/";
			app += module_root_path;
		}

		parse_module_path(&_syntax_tree, _main_module, app, &state);
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
	for (auto i: state.get_imports())
	{
		auto cs = new async_data(&_syntax_tree);
		const auto m = get_module(i);
		if (m != nullptr)
		{
			auto sources = m->imported_files(i);
			try_import(cs, m, sources->relative_path, sources);
		}
	}

	// handle each parse request and import statement
	while (_parse_responses.is_open() && _import_statements_running > 0)
	{
		async_data* cs;
		if (!_parse_responses.try_pop(&cs))
			continue;

		// One import statement is done!
		_import_statements_running--;

		// Did an error happen when we parsed the source code?
		if (!cs->errors.empty())
		{
			// the load failed
			cs->sources->load_status = package_source_code::failed;

			//for (const auto& e: cs->errors)
			//	std::cerr << e << std::endl;
			//cs->errors.clear();
			success = false;
		}
		else if (_config.verbose_level > 0)
		{
			if (cs->package_name.empty())
				std::cout << "parsed '" << cs->module->get_name() << "' - done!" << std::endl;
			else
				std::cout << "parsed '" << cs->module->get_name() << "/" << cs->package_name << "' - done!"
						  << std::endl;
		}

		if (cs->package)
		{
			// the load completed successfully
			cs->sources->load_status = package_source_code::successful;

			// add the package to the module
			assert(cs->module);
			cs->module->add_child(cs->package);
		}

		// collect all imports to be parsed
		auto imports = cs->state.get_imports();
		for (auto i: imports)
		{
			const auto m = get_module(i);
			if (m)
			{
				auto sources = m->imported_files(i);
				if (cs == nullptr)
					cs = new async_data(&_syntax_tree);
				if (try_import(cs, m, sources->relative_path, sources))
					cs = nullptr;
			}
		}
		// delete any state that's pending
		delete cs;
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
		// TODO: move resolve phase to the parser thread
		try
		{
			resolve(&_syntax_tree, &state);
			optimize(&_syntax_tree, 0);
		}
		catch (const o2::error& e)
		{
			// print errors, but continue evaluating all other source code
			// because it's tedious to having to fix one problem at a time
			e.print(std::cerr);
			success = false;
			_syntax_tree.debug();
		}
	}

	const auto diff = now() - start;
	if (_config.verbose_level > 0)
		std::cout << "build took " << diff << " milliseconds" << std::endl;
	return success ? 0 : 1;
}

bool build::try_import(async_data* cs, module* m, string_view package_name,
		package_source_code* sources)
{
	// we only care about package sources that's not loaded
	if (sources->load_status != package_source_code::not_loaded)
		return false;

	// TODO: figure out a better way of doing this!
	_import_statements_running++;

	// initialize the async data
	cs->errors.clear();
	cs->module = m;
	cs->package_name = package_name;
	cs->sources = sources;
	cs->sources->load_status = package_source_code::loading;
	cs->package = nullptr;

	_parse_requests.put(cs);
	return true;
}

async_data* build::parse(async_data* cs)
{
	try
	{
		cs->package = parse_module_import(cs->sources->sources, cs->package_name, &cs->state);
	}
	catch (const error& e)
	{
		std::stringstream ss;
		e.print(ss);
		cs->errors.emplace_back(std::move(ss.str()));
	}
	catch (const std::exception& e)
	{
		cs->errors.emplace_back(e.what());
	}
	return cs;
}

o2::module* build::get_module(string_view import_statement)
{
	// search among the required modules first
	module* best_match = nullptr;
	int match = _main_module->matches(import_statement);
	for (auto m: _main_module->get_requirements())
	{
		const auto current_match = m->matches(import_statement);
		// -1 will become super large by doing this, thus no need to check for -1
		if ((unsigned int)current_match < (unsigned int)match)
		{
			best_match = m;
			match = current_match;
		}
	}

	if (best_match)
		return best_match;

	// If not then check among the built-in modules
	const auto it = _builtin_modules.find(import_statement);
	if (it == _builtin_modules.end())
	{
		// see if there's a directory in the language folder that might have modules in them
		std::filesystem::path path(_config.lang_path);
		path = path / std::filesystem::path(import_statement);
		if (std::filesystem::is_directory(path))
		{
			best_match = o2_new module(string(import_statement),
					path.generic_string(),
					new filesystem_module_source_codes(path.generic_string()));
			_builtin_modules[import_statement] = best_match;
			_syntax_tree.get_root_package()->add_child(best_match);
		}
	}
	else
		best_match = it->second;

	return best_match;
}
