//
// Part of the o2 Project, under the Apache License v2.0 with o2 Project Exceptions.
// See the LICENSE file in the project root for license terms
//

#pragma once

#include "../parser/parser.h"
#include "../parser/module/module_package_lookup.h"
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

	static void import_package_sources_this(module* const mod, syntax_tree* const st, vector<node_import*> imports)
	{
		int imports_count = imports.size();
		for (auto i: imports)
		{
			const auto imported_module = mod->find_module(i->get_import_statement());
			if (imported_module)
			{
				const auto sources = imported_module->get_package_info(i->get_import_statement());
				if (sources->load_status == package_source_info::not_loaded)
				{
					imported_module->load_package_sources(sources);
					sources->load_status = package_source_info::loading;
					parser_state ps0(st);
					const auto imported_package = parse_package_sources(sources->sources, sources->name, &ps0);
					if (imported_package)
						imported_module->add_package(imported_package);
					sources->load_status = package_source_info::successful;
					imported_module->notify_package_imported(imported_package);
					import_package_sources_this(imported_module, st, std::move(ps0.get_imports()));
					imported_package->process_phases();
				}
				else if (sources->load_status == package_source_info::successful)
				{
					i->notify_imported();
					imports_count--;
				}
			}
		}
	}

	static void parse_package_sources_this(module* main_module, syntax_tree* const st, string_view package_name)
	{
		parser_state ps(st);
		auto package = parse_main_module_package(main_module, package_name, &ps);
		if (package == nullptr)
			return;

		auto imports = ps.get_imports();
		if (imports.empty())
		{
			// package is now imported
			main_module->notify_package_imported(package);
		}
		else
			import_package_sources_this(main_module, st, std::move(imports));

		// all imports are imported, so let's resolve this package's dependencies!
		package->process_phases();
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
			system_modules* sm;
			module* m;
			try
			{
				llvm::LLVMContext lcontext;
				st = new syntax_tree(lcontext);
				sm = new system_modules(std::filesystem::path("./lang"), st);
				m = o2_new module(sm, module_name, path);
				m->insert_into(st);

				parse_package_sources_this(m, st, app);
				optimize(st, 0);
				t(*st);
				delete m;
				delete sm;
				delete st;
			}
			catch (const o2::error& e)
			{
				if (test_state::debugging())
					e.print(std::cout);
				if (test_state::debugging())
					debug(*st);
				delete m;
				delete sm;
				delete st;
				memory_tracker::end();
				throw;
			}
			catch (const std::exception&)
			{
				if (test_state::debugging())
					debug(*st);
				delete m;
				delete sm;
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
			system_modules* sm;
			module* m;
			try
			{
				llvm::LLVMContext lcontext;
				st = new syntax_tree(lcontext);
				sm = new system_modules(std::filesystem::path("./lang"), st);
				m = o2_new module(sm, module_name, path);
				m->insert_into(st);

				o2::parser_state state(st);
				parse_package_sources_this(m, st, app);
				optimize(st, 0);
				fail("expected error exception");
			}
			catch (const assertion_error&)
			{
				if (test_state::debugging())
					debug(*st);
				delete m;
				delete sm;
				delete st;
				throw;
			}
			catch (const std::exception& e)
			{
				try
				{
					t(e);
					delete m;
					delete sm;
					delete st;
					memory_tracker::end();
				}
				catch (const assertion_error&)
				{
					if (test_state::debugging())
						debug(*st);
					delete m;
					delete sm;
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