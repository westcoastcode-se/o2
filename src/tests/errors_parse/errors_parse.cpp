//
// Part of the o2 Project, under the Apache License v2.0 with o2 Project Exceptions.
// See the LICENSE file in the project root for license terms
//

#include "../utils.h"

using namespace std;
using namespace o2;

void errors_parse()
{
	static const string_view ROOT_PATH("src/tests/errors_parse");
	suite("errors_parse", []()
	{
		test_error("extern_func_with_body", ROOT_PATH, [](const std::exception& e)
		{
			const auto e2 = assert_type<error_unexpected_extern_func_body>(&e);
			assert_equals(string_view(STR("unexpected extern function body")), e2->get_error());
		});
		test_error("func_arguments_not_closed", ROOT_PATH, [](const std::exception& e)
		{
			const auto e2 = assert_type<error_expected_identity>(&e);
			assert_equals(string_view(STR("expected 'identity' but was '{'")), e2->get_error());
		});
		test_error("func_arguments_not_closed2", ROOT_PATH, [](const std::exception& e)
		{
			const auto e2 = assert_type<error_syntax_error>(&e);
			assert_equals(string_view(STR("expected ')' or ',' but was '{'")), e2->get_error());
		});
		test_error("func_arguments_not_closed3", ROOT_PATH, [](const std::exception& e)
		{
			const auto e2 = assert_type<error_expected_identity>(&e);
			assert_equals(string_view(STR("expected 'identity' but was '{'")), e2->get_error());
		});
		test_error("func_argument_type_specified", ROOT_PATH, [](const std::exception& e)
		{
			const auto e2 = assert_type<error_expected_identity>(&e);
			assert_equals(string_view(STR("expected 'identity' but was '{'")), e2->get_error());
		});
		test_error("func_scope_not_closed", ROOT_PATH, [](const std::exception& e)
		{
			const auto e2 = assert_type<error_syntax_error>(&e);
			assert_equals(string_view(STR("expected '}' but was <EOF>")), e2->get_error());
		});
		test_error("func_scope_scope_not_closed", ROOT_PATH, [](const std::exception& e)
		{
			const auto e2 = assert_type<error_syntax_error>(&e);
			assert_equals(string_view(STR("expected '}' but was <EOF>")), e2->get_error());
		});
	});

}