//
// Part of the o2 Project, under the Apache License v2.0 with o2 Project Exceptions.
// See the LICENSE file in the project root for license terms
//

#include <iostream>
#include <string_view>
#include <string>
#include <fstream>

using namespace std;

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
		cout << "\ttest\t\ttest packages" << endl;
		return 0;
	}

	const string_view command(argv[1]);
	static const string_view BUILD("build");
	static const string_view INIT("init");
	static const string_view TEST("test");

	if (command == BUILD)
	{
		ifstream mod_file("o2.mod");
		if (!mod_file.is_open())
		{
			cerr << "no o2.mod found" << endl;
			return 1;
		}

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
		return 0;
	}

	return 0;
}