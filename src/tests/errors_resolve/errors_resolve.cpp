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
			const auto e2 = assert_type<resolve_error_recursion>(&e);
			assert_equals(string_view(
							STR("recursion detected: '/westcoastcode.se/tests/Item->/westcoastcode.se/tests/Item/item->/westcoastcode.se/tests/Item'")),
					e2->get_error());
		});
		test_error("field_in_struct_recursion2", ROOT_PATH, [](const std::exception& e)
		{
			const auto e2 = assert_type<resolve_error_recursion>(&e);
			assert_equals(string_view(
							STR("recursion detected: '/westcoastcode.se/tests/Node->/westcoastcode.se/tests/Node/list->/westcoastcode.se/tests/List->/westcoastcode.se/tests/List/head->/westcoastcode.se/tests/Node'")),
					e2->get_error());
		});
		test_error("duplicated_type", ROOT_PATH, [](const std::exception& e)
		{
			const auto e2 = assert_type<error_named_symbol_already_declared>(&e);
			assert_equals(string_view(STR("symbol 'Name' is already declared")), e2->get_error());
		});
		test_error("duplicated_type_in_struct", ROOT_PATH, [](const std::exception& e)
		{
			const auto e2 = assert_type<error_named_symbol_already_declared>(&e);
			assert_equals(string_view(STR("symbol 'Name' is already declared")), e2->get_error());
		});
		test_error("duplicated_func_with_body", ROOT_PATH, [](const std::exception& e)
		{
			const auto e2 = assert_type<error_named_symbol_already_declared>(&e);
			assert_equals(string_view(STR("symbol 'f' is already declared")), e2->get_error());
		});
		test_error("duplicated_extern_func", ROOT_PATH, [](const std::exception& e)
		{
			const auto e2 = assert_type<error_named_symbol_already_declared>(&e);
			assert_equals(string_view(STR("symbol 'f' is already declared")), e2->get_error());
		});
		test_error("duplicated_func_in_func", ROOT_PATH, [](const std::exception& e)
		{
			const auto e2 = assert_type<error_named_symbol_already_declared>(&e);
			assert_equals(string_view(STR("symbol 'f1' is already declared")), e2->get_error());
		});
	});

}