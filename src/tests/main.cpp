//
// Part of the o2 Project, under the Apache License v2.0 with o2 Project Exceptions.
// See the LICENSE file in the project root for license terms
//

#include "test.h"

extern void packages();

extern void funcs();

extern void imports();

extern void modules();

extern void structs();

extern void errors_parse();

extern void errors_resolve();

/*
int run_tests()
{
	test_state::stop_suit_on_error() = true;
	test_state::debugging() = true;

	suite("resolving", []()
	{
		const auto source = R"(package inner.pkg;func f()custom_type{}package inner.pkg;type custom_type{})";
		ptest("resolving a custom type in same package but different package block", source, [](syntax_tree& st)
		{
			st.resolve();
			const auto root = st.get_root_package();

			const auto inner_package = assert_type<node_package>(root->get_child(13));
			const auto inner_pkg_package = assert_type<node_package>(inner_package->get_child(0));
			const auto func = assert_type<node_func_def>(inner_pkg_package->get_child(0));
		});
		const auto source2 = R"(package omg; package inner.pkg;import omg;func f()custom_type{}package inner.pkg;type custom_type{})";
		ptest("resolving a custom type in same package but different package block under an import", source2,
				[](syntax_tree& st)
				{
					st.resolve();
					const auto root = st.get_root_package();

					const auto inner_package = assert_type<node_package>(root->get_child(14));
					const auto inner_pkg_package = assert_type<node_package>(inner_package->get_child(0));
					const auto import_omg = assert_type<node_import>(inner_pkg_package->get_child(0));
					const auto func = assert_type<node_func_def>(import_omg->get_child(1));
				});
	});
	suite("function resolving", []()
	{
		ptest("inner func duplication", "func f() { { func f2() {} } func f2() {} }",
				[](syntax_tree& st)
				{
					st.resolve();
					const auto root = st.get_root_package();
					assert_equals(root->get_children().size(), 14);
					const auto f = assert_type<node_func_def>(root->get_children()[13]);
					assert_equals(f->get_name(), "f");
					assert_equals(f->get_children().size(), 3);

					const auto f_body = assert_type<node_func_body>(f->get_children()[2]);
					const auto f_body_scope1 = assert_type<node_scope>(f_body->get_children()[0]);

					const auto f_body_scope1_scope1 = assert_type<node_scope>(f_body_scope1->get_children()[0]);
					const auto f_body_scope1_scope1_f2 = assert_type<node_func_def>(
							f_body_scope1_scope1->get_children()[0]);
					assert_equals(f_body_scope1_scope1_f2->get_name(), "f2");

					const auto f_body_scope1_f2 = assert_type<node_func_def>(f_body_scope1->get_children()[1]);
					assert_equals(f_body_scope1_f2->get_name(), "f2");
				});
	});
	suite("errors", []()
	{
		test_exception("func argument type expected", "package name; func name(arg_name {}",
				[](const std::exception& e)
				{
					auto ee = assert_type<o2::error_expected_identity>(&e);
					assert_equals(ee->get_code(), error_types::expected_identity);
				});
		test_exception("function name argument missing end 1", "package name; func name(arg int{}",
				[](const std::exception& e)
				{
					auto ee = assert_type<o2::error_syntax_error>(&e);
					assert_equals(ee->get_code(), error_types::syntax_error);
				});
		test_exception("function name argument missing end 2", "package name; func name(arg int,{}",
				[](const std::exception& e)
				{
					auto ee = assert_type<o2::error_expected_identity>(&e);
					assert_equals(ee->get_code(), error_types::expected_identity);
				});
		test_exception("function missing scope end", "func f(){",
				[](const std::exception& e)
				{
					auto ee = assert_type<o2::error_syntax_error>(&e);
					assert_equals(ee->get_code(), error_types::syntax_error);
				});
		test_exception("function missing scope end after inner scope", "func f(){{}",
				[](const std::exception& e)
				{
					auto ee = assert_type<o2::error_syntax_error>(&e);
					assert_equals(ee->get_code(), error_types::syntax_error);
				});
	});

	return test_state::success() ? 0 : 1;
}
*/
int main(int argc, char** argv)
{
	o2::testing::test_state::stop_suit_on_error() = true;
	o2::testing::test_state::debugging() = true;

	std::string ignore;
	if (argc > 1)
		ignore = argv[1];
	if (argc > 2)
	{
		ignore += '.';
		ignore += argv[2];
	}
	if (!ignore.empty())
		o2::testing::test_state::test_name() = ignore;

	packages();
	imports();
	modules();
	funcs();
	structs();
	errors_parse();
	errors_resolve();

	return o2::testing::test_state::success() ? 0 : 1;
}
