//
// Part of the o2 Project, under the Apache License v2.0 with o2 Project Exceptions.
// See the LICENSE file in the project root for license terms
//

#include <iostream>
#include <string_view>
#include <unordered_map>
#include <fstream>

#include "../parser/parser.h"
#include "../parser/resolver.h"
#include "../parser/module/module_source_code.h"

using namespace std;
using namespace o2;

bool module_exists()
{
	ifstream check("o2.mod");
	if (check.is_open())
	{
		check.close();
		return true;
	}
	return false;
}

int build(module* m, string_view path)
{
	// convert the "path" where the main source code is found in the module
	// into a module-relative path
	string app(m->get_name());
	if (path.length() > 0)
	{
		app += "/";
		app += path;
	}

	// TODO add support for a smarted standard lang module imports
	unordered_map<string_view, node_module*> _builtin_modules;
	_builtin_modules["stdio"] = o2_new node_module(source_code_view(),
			o2_new module(string_view("stdio"),
					string_view("../lang/stdio"),
					new filesystem_module_source_codes(string("../lang/stdio"))));

	syntax_tree* st;
	try
	{
		st = new syntax_tree;

		// Initialize the module node and add it to the syntax tree
		const auto nm = o2_new node_module(source_code_view(), m);
		st->get_root_package()->add_child(nm);
		// Parse the source code
		parser_state state(st);
		parse_module_path(st, m, app, &state);
		// TODO: add support for threads
		auto imports = state.get_imports();
		while (!imports.empty())
		{
			for (auto i: imports)
			{
				// TODO: figure out the module based on the import statement
				const auto builtin_it = _builtin_modules.find(i->get_import_statement());
				if (builtin_it != _builtin_modules.end())
				{
					// TODO figure out the path to where lang is located
					if (builtin_it->second->get_parent() == nullptr)
					{
						// add the module
						st->get_root_package()->add_child(builtin_it->second);
					}
					// TODO: This should be done in a "compiler" pipeline so that we can use multiple threads
					auto package = parse_module_import(st, builtin_it->second->get_module(),
							i->get_import_statement(),
							&state);
					if (package)
						builtin_it->second->add_child(package);
				}
				else
				{
					auto package = parse_module_import(st, m, i->get_import_statement(),
							&state);
					if (package)
						nm->add_child(package);
				}
			}
			imports = std::move(state.get_imports());
		}
		resolve(st, &state);
		optimize(st, 0);
		st->debug();
		delete st;
	}
	catch (const o2::error& e)
	{
		e.print(cerr);
		delete st;
	}
	catch (const std::exception& e)
	{
		cerr << "unhandled exception: '" << e.what() << "'" << endl;
		delete st;
	}
	return 0;
}

int main(int argc, char** argv)
{
	if (argc < 2)
	{
		cout << "o2 is a tool for managing o2 source code." << endl << endl;
		cout << "usage: " << endl << endl;
		cout << "\to2 <command> [arguments]" << endl << endl;
		cout << "The commands are:" << endl << endl;
		cout << "\tbuild\t\tcompile packages and dependencies" << endl;
		cout << "\tinit\t\tinitialize a new project" << endl;
		cout << "\tparse\t\tparse source code into a code completion database" << endl;
		cout << "\ttest\t\ttest packages" << endl;
		return 0;
	}

	const string_view command(argv[1]);
	static const string_view BUILD("build");
	static const string_view INIT("init");
	static const string_view PARSE("parse");
	static const string_view TEST("test");

	if (command == BUILD)
	{
		if (argc < 3)
		{
			cout << "o2 build provides functionality compile o2 source code" << endl << endl;
			cout << "usage: " << endl << endl;
			cout << "\to2 build <main source code path>" << endl << endl;
			return 0;
		}

		ifstream mod_file("o2.mod");
		if (!mod_file.is_open())
		{
			cerr << "no o2.mod found" << endl;
			return 1;
		}

		const string_view root_path(argv[2]);
		string_view module_root_path = root_path;
		if (module_root_path == ".")
			module_root_path = string_view();
		mod_file.close();

		// TODO: Parse module file and figure out the module name form that

		memory_tracker::begin();
		const string_view module_name("westcoastcode.se/hello_world");
		const auto m = o2_new module(module_name, module_root_path,
				new filesystem_module_source_codes(root_path));
		const auto ret = build(m, module_root_path);
		memory_tracker::end();
		return ret;
	}
	else if (command == PARSE)
	{
		if (argc < 3)
		{
			cout << "o2 parse provides functionality that turns source code into a code completion database";
			cout << endl << endl;
			cout << "usage: " << endl << endl;
			cout << "\to2 parse <main source code path>" << endl << endl;
			return 0;
		}

		ifstream mod_file("o2.mod");
		if (!mod_file.is_open())
		{
			cerr << "no o2.mod found" << endl;
			return 1;
		}

		const string_view main_path(argv[2]);


		mod_file.close();
	}
	else if (command == INIT)
	{
		if (module_exists())
		{
			cerr << "o2.mod already exists" << endl;
			return 1;
		}

		cout << "module name:\t";
		string module_name;
		cin >> module_name;

		ofstream mod_file("o2.mod");
		mod_file << "compiler 1.0" << endl;
		mod_file << endl;
		mod_file << "name \"" << module_name << "\"" << endl;
		mod_file.close();
		return 0;
	}

	return 0;
}