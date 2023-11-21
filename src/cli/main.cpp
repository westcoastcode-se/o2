//
// Part of the o2 Project, under the Apache License v2.0 with o2 Project Exceptions.
// See the LICENSE file in the project root for license terms
//

#include <iostream>
#include <string_view>
#include <fstream>
#include <chrono>
#include <csignal>

#include "commands/build.h"

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

base_command* bcommand = nullptr;

void abort_command(int sig)
{
	if (bcommand)
		bcommand->abort();
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

	const o2::string_view command(argv[1]);
	static const o2::string_view BUILD(STR("build"));
	static const o2::string_view INIT(STR("init"));
	static const o2::string_view PARSE(STR("parse"));
	static const o2::string_view TEST(STR("test"));

	signal(SIGINT, abort_command);

	if (command == BUILD)
	{
		if (argc < 3)
		{
			cout << "o2 build provides functionality compile o2 source code" << endl << endl;
			cout << "usage: " << endl << endl;
			cout << "\to2 build <main source code path>" << endl << endl;
			return 0;
		}

		if (!module_exists())
		{
			cerr << "no o2.mod found" << endl;
			return 1;
		}

		// TODO: add support for compiler flags, such as:
		// -o destination
		// -t json|binary|debug|library

		const string_view root_path(argv[2]);
		o2::build b(build::config{
				std::filesystem::path(root_path),
				std::filesystem::path("../lang"),
				1,
				5,
				build_config_output::json,
				""
		});
		bcommand = &b;
		return b.execute();
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