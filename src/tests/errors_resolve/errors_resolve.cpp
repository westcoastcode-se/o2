//
// Part of the o2 Project, under the Apache License v2.0 with o2 Project Exceptions.
// See the LICENSE file in the project root for license terms
//

#include "../utils.h"

using namespace std;
using namespace o2;

void errors_resolve()
{
	static const string_view ROOT_PATH("src/tests/errors_resolve");
	suite("errors_resolve", []()
	{
		test_error("field_in_struct_recursion", ROOT_PATH, [](const std::exception& e)
		{
			assert_type<resolve_error_recursion>(&e);
			assert_equals(string_view("recursion detected: '.Item->.Item->.Item'"), e.what());
		});
		test_error("field_in_struct_recursion2", ROOT_PATH, [](const std::exception& e)
		{
			assert_type<resolve_error_recursion>(&e);
			assert_equals(string_view("recursion detected: '.List->.Node->.List'"), e.what());
		});
		test_error("duplicated_type", ROOT_PATH, [](const std::exception& e)
		{
			assert_type<error_named_symbol_already_declared>(&e);
			assert_equals(string_view("symbol '.Name' is already declared"), e.what());
		});
		test_error("duplicated_type_in_struct", ROOT_PATH, [](const std::exception& e)
		{
			assert_type<error_named_symbol_already_declared>(&e);
			assert_equals(string_view("symbol '.Name' is already declared"), e.what());
		});
		test_error("duplicated_func_with_body", ROOT_PATH, [](const std::exception& e)
		{
			assert_type<error_named_symbol_already_declared>(&e);
			assert_equals(string_view("symbol '.f' is already declared"), e.what());
		});
		test_error("duplicated_extern_func", ROOT_PATH, [](const std::exception& e)
		{
			assert_type<error_named_symbol_already_declared>(&e);
			assert_equals(string_view("symbol '.f' is already declared"), e.what());
		});
		test_error("duplicated_func_in_func", ROOT_PATH, [](const std::exception& e)
		{
			assert_type<error_named_symbol_already_declared>(&e);
			assert_equals(string_view("symbol '.f1' is already declared"), e.what());
		});
	});

}