//
// Part of the o2 Project, under the Apache License v2.0 with o2 Project Exceptions.
// See the LICENSE file in the project root for license terms
//

#include "build.h"
#include <sstream>
#include <iostream>

using namespace o2;

build::build(config cfg)
		: _config(cfg), _import_statements_running(0)
{
	// TODO: use config
	_builtin_modules["stdio"] = o2_new node_module(source_code_view(),
			o2_new module(string_view("stdio"),
					string_view("../lang/stdio"),
					new filesystem_module_source_codes(string("../lang/stdio"))));
	_builtin_modules["stdlib"] = o2_new node_module(source_code_view(),
			o2_new module(string_view("stdlib"),
					string_view("../lang/stdlib"),
					new filesystem_module_source_codes(string("../lang/stdlib"))));
}

build::~build()
{
	assert(!_channel.is_open());
}

int build::execute()
{
	string_view module_root_path = _config.path;
	if (module_root_path == ".")
		module_root_path = string_view();

	// TODO: Parse module file in the config root path and figure out the module name form that
	const string_view module_name("westcoastcode.se/hello_world");
	_main_module = o2_new module(module_name, module_root_path,
			new filesystem_module_source_codes(_config.path));

	// Initialize the module we are compiling and add it to the syntax tree
	const auto nm = o2_new node_module(source_code_view(), _main_module);
	_syntax_tree.get_root_package()->add_child(nm);
	//
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
		auto cs = new compile_state(new parser_state(&_syntax_tree));
		bool loaded = false;
		string_view package_name;
		auto sources = get_sources(i, &package_name, &loaded);
		if (loaded)
			do_import(cs->init(package_name, i, sources));
	}

	// handle each parse request and import statement
	while (_channel.is_open() && _import_statements_running > 0)
	{
		compile_state* cs;
		if (!_channel.try_pop(&cs, std::chrono::microseconds(0)))
			continue;

		// One import statement is done!
		_import_statements_running--;

		// Did an error happen when we parsed the source code?
		if (!cs->errors.empty())
		{
			for (const auto& e: cs->errors)
				std::cerr << e << std::endl;
			cs->errors.clear();
			success = false;
		}
		else if (_config.verbose_level > 0)
		{
			std::cout << "parsed '" << cs->import_statement << "' - done!" << std::endl;
		}

		// collect all imports to be parsed
		for (auto i: cs->state->get_imports())
		{
			bool loaded = false;
			string_view package_name;
			auto sources = get_sources(i, &package_name, &loaded);
			if (!loaded)
				continue;

			if (cs == nullptr)
				cs = new compile_state(new parser_state(&_syntax_tree));
			do_import(cs->init(package_name, i, sources));
			// ownership is now in the import block
			cs = nullptr;
		}
		// delete any state that's pending
		delete cs;
	}
	_channel.close();

	if (success)
	{
		// TODO: move resolve phase to the parser thread
		resolve(&_syntax_tree, &state);
		optimize(&_syntax_tree, 0);
		_syntax_tree.debug();
	}

	return success ? 0 : 1;
}

void build::do_import(compile_state* cs)
{
	_import_statements_running++;
	auto* ch = &_channel;
	std::async(std::launch::async, [ch, cs]()
	{
		ch->put(build::parse(cs));
	});
}

array_view<source_code*> build::get_sources(string_view import_statement, string_view* package_name, bool* loaded)
{
	// TODO: Figure out the module based on the import statement in a smarter way
	const auto builtin_it = _builtin_modules.find(import_statement);
	if (builtin_it != _builtin_modules.end())
	{
		auto result = builtin_it->second->get_module()->imported_files(import_statement, loaded);
		if (*loaded)
		{
			*package_name = builtin_it->second->get_module()->get_relative_path(import_statement);
			_syntax_tree.get_root_package()->add_child(builtin_it->second);
		}
		return result;
	}
	else
	{
		// TODO: get the appropriate module. Assume that we are parsing the main module for now
		*package_name = _main_module->get_relative_path(import_statement);
		return _main_module->imported_files(import_statement, loaded);
	}
}

compile_state* build::parse(compile_state* cs)
{
	try
	{
		cs->package = parse_module_import(cs->sources, cs->package_name, cs->state);
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
