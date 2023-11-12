//
// Part of the o2 Project, under the Apache License v2.0 with o2 Project Exceptions.
// See the LICENSE file in the project root for license terms
//

#pragma once

#include "../parser/parser.h"
#include "../parser/resolver.h"
#include "../parser/module/module_source_code.h"
#include "test.h"

using namespace o2;
using namespace o2::testing;

namespace o2
{
	static inline void debug(const syntax_tree& st)
	{
		std::cout << std::endl;
		st.debug();
		std::cout << std::endl;
	}

	static void test(string_view name, string_view root_path, string_view app_path, std::function<void(syntax_tree&)> t)
	{
		// run the test
		o2::testing::test(name, [t = std::move(t), name, root_path, app_path]()
		{
			static const string_view module_name("westcoastcode.se/tests");

			// root path will be, for example, "tests/packages/empty"
			string path(root_path);
			path += '/';
			path += name;

			string app(module_name);
			if (!app_path.empty())
			{
				app += '/';
				app += app_path;
			}

			memory_tracker::begin();

			syntax_tree* st;
			try
			{
				st = new syntax_tree;
				// TODO parse module project file and pre_load all modules and put them into
				//      the syntax tree
				const auto m = o2_new
						module(module_name, path,
						new filesystem_module_source_codes(path));
				const auto nm = o2_new node_module(source_code_view(), m);
				st->get_root_package()->add_child(nm);

				o2::parser_state state(st);
				parse_module_path(st, m, string_view(app), &state);

				// TODO add support for required modules.
				auto imports = state.get_imports();
				while (!imports.empty())
				{
					for (auto i: imports)
					{
						auto package = parse_module_import(st, m, i, &state);
						if (package)
							nm->add_child(package);
					}
					imports = std::move(state.get_imports());
				}
				resolve(st, &state);
				optimize(st, 0);
				t(*st);
				delete st;
			}
			catch (const o2::error& e)
			{
				if (test_state::debugging())
					e.print(std::cout);
				if (test_state::debugging())
					debug(*st);
				delete st;
				memory_tracker::end();
				throw;
			}
			catch (const std::exception&)
			{
				if (test_state::debugging())
					debug(*st);
				delete st;
				memory_tracker::end();
				throw;
			}
			if (!memory_tracker::end())
				fail("memory leak detected");
		});
	}

	static void test(string_view name, string_view root_path, std::function<void(syntax_tree&)> t)
	{
		test(name, root_path, string_view(), std::move(t));
	}

	static void
	test(std::initializer_list<string_view> names, string_view root_path, std::function<void(syntax_tree&)> t)
	{
		for (auto name: names)
		{
			test(name, root_path, string_view(), t);
		}
	}

	static void
	test_error(string_view name, string_view root_path, string_view app_path,
			std::function<void(const std::exception&)> t)
	{
		// run the test
		o2::testing::test(name, [t = std::move(t), name, root_path, app_path]()
		{
			static const string_view module_name("westcoastcode.se/tests");

			// root path will be, for example, "tests/packages/empty"
			string path(root_path);
			path += '/';
			path += name;

			string app(module_name);
			if (!app_path.empty())
			{
				app += '/';
				app += app_path;
			}

			memory_tracker::begin();
			syntax_tree* st;
			try
			{
				st = new syntax_tree;
				const auto m = o2_new
						module(module_name, path,
						new filesystem_module_source_codes(path));
				const auto nm = o2_new node_module(source_code_view(), m);
				st->get_root_package()->add_child(nm);

				o2::parser_state state(st);
				parse_module_path(st, m, string_view(app), &state);

				// TODO add support for required modules.
				auto imports = state.get_imports();
				while (!imports.empty())
				{
					for (auto i: imports)
					{
						auto package = parse_module_import(st, m, i, &state);
						if (package)
							nm->add_child(package);
					}
					imports = std::move(state.get_imports());
				}
				resolve(st, &state);
				optimize(st, 0);
				fail("expected error exception");
			}
			catch (const assertion_error&)
			{
				if (test_state::debugging())
					debug(*st);
				delete st;
				throw;
			}
			catch (const std::exception& e)
			{
				try
				{
					t(e);
					delete st;
					memory_tracker::end();
				}
				catch (const assertion_error&)
				{
					if (test_state::debugging())
						debug(*st);
					delete st;
					memory_tracker::end();
					throw;
				}
			}
			if (!memory_tracker::end())
				fail("memory leak detected");
		});
	}

	static void test_error(string_view name, string_view root_path, std::function<void(const std::exception&)> t)
	{
		test_error(name, root_path, string_view(), std::move(t));
	}
}