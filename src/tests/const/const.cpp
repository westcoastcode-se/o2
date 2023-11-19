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
			assert_equals(root->get_children().size(), 14);

			const auto project_module = assert_type<node_module>(root->get_child(13));
			assert_equals(project_module->get_child_count(), 1);
			const auto package_main = assert_type<node_package>(project_module->get_child(0));

			const auto var_INT = assert_type<node_var>(package_main->get_child(0));
			assert_equals(var_INT->get_name(), "INT");
			const auto implicit_INT = assert_type<node_type_implicit>(var_INT->get_type());
			assert_equals(implicit_INT->get_type(), root->get_child(7));

			const auto var_INT2 = assert_type<node_var>(package_main->get_child(1));
			assert_equals(var_INT2->get_name(), "INT2");
			const auto ref_INT2 = assert_type<node_type_ref>(var_INT2->get_type());
			assert_equals(ref_INT2->get_type(), root->get_child(7));

			const auto var_FLOAT = assert_type<node_var>(package_main->get_child(2));
			assert_equals(var_FLOAT->get_name(), "FLOAT");
			const auto implicit_FLOAT = assert_type<node_type_implicit>(var_FLOAT->get_type());
			assert_equals(implicit_FLOAT->get_type(), root->get_child(11));

			const auto var_FLOAT2 = assert_type<node_var>(package_main->get_child(3));
			assert_equals(var_FLOAT2->get_name(), "FLOAT2");
			const auto ref_FLOAT2 = assert_type<node_type_ref>(var_FLOAT2->get_type());
			assert_equals(ref_FLOAT2->get_type(), root->get_child(11));

			const auto var_FLOAT3 = assert_type<node_var>(package_main->get_child(4));
			assert_equals(var_FLOAT3->get_name(), "FLOAT3");
			const auto ref_FLOAT3 = assert_type<node_type_ref>(var_FLOAT3->get_type());
			assert_equals(ref_FLOAT3->get_type(), root->get_child(11));
		});
		test("const_func", ROOT_PATH, [](syntax_tree& st)
		{
			const auto root = st.get_root_package();
			assert_equals(root->get_children().size(), 14);
			const auto project_module = assert_type<node_module>(root->get_child(13));
			assert_equals(project_module->get_child_count(), 1);
			const auto package_main = assert_type<node_package>(project_module->get_child(0));

			const auto func = assert_type<node_func>(package_main->get_child(0));
			assert_equals(func->get_name(), "Get5");
			assert_not_null(func->get_body());
			assert_equals(func, func->get_body()->get_def());
			assert_not_null(func->get_arguments());
			assert_equals(func->get_arguments()->num_arguments(), 0);
			const auto ret = assert_not_null(func->get_returns());
			assert_equals(ret->get_children().size(), 1);
			const auto ret_ref_int = assert_type<node_type_ref>(ret->get_child(0));
			assert_equals(ret_ref_int->get_size(), sizeof(int));
			assert_equals(ret_ref_int->get_type(), root->get_child(7));
			const auto ref_int = assert_type<node_ref>(ret_ref_int->get_child(0));
			assert_equals(ref_int->get_query_text(), "int");
			const auto body = assert_type<node_func_body>(func->get_child(2));
			assert_equals(body->get_children().size(), 1);
			assert_type<node_scope>(body->get_children()[0]);
		});
	});
}
