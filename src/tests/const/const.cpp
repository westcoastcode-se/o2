//
// Part of the o2 Project, under the Apache License v2.0 with o2 Project Exceptions.
// See the LICENSE file in the project root for license terms
//

#include "../utils.h"
#include "../../parser/types/node_type_implicit.h"

using namespace std;
using namespace o2;

void const_()
{

	static const string_view ROOT_PATH("src/tests/const");

	suite("const", []()
	{
		test("const_global", ROOT_PATH, [](syntax_tree& st)
		{
			const auto root = st.get_root_package();
			assert_equals(root->get_children().size(), 18);

			const auto project_module = assert_type<node_module>(root->get_child(13));
			assert_equals(project_module->get_name(), "westcoastcode.se/tests");

			const auto var_INT = assert_type<node_var>(root->get_child(14));
			assert_equals(var_INT->get_name(), "INT");
			const auto implicit_INT = assert_type<node_type_implicit>(var_INT->get_type());
			assert_equals(implicit_INT->get_type(), root->get_child(7));

			const auto var_INT2 = assert_type<node_var>(root->get_child(15));
			assert_equals(var_INT2->get_name(), "INT2");
			const auto ref_INT2 = assert_type<node_type_ref>(var_INT2->get_type());
			assert_equals(ref_INT2->get_type(), root->get_child(7));

			const auto var_FLOAT = assert_type<node_var>(root->get_child(16));
			assert_equals(var_FLOAT->get_name(), "FLOAT");
			const auto implicit_FLOAT = assert_type<node_type_implicit>(var_FLOAT->get_type());
			assert_equals(implicit_FLOAT->get_type(), root->get_child(11));

			const auto var_FLOAT2 = assert_type<node_var>(root->get_child(17));
			assert_equals(var_FLOAT2->get_name(), "FLOAT2");
			const auto ref_FLOAT2 = assert_type<node_type_ref>(var_FLOAT2->get_type());
			assert_equals(ref_FLOAT2->get_type(), root->get_child(11));

			debug(st);
		});
	});
}
