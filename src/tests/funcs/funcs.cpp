//
// Part of the o2 Project, under the Apache License v2.0 with o2 Project Exceptions.
// See the LICENSE file in the project root for license terms
//

#include "../utils.h"

using namespace std;
using namespace o2;

void funcs()
{
	static const string_view ROOT_PATH("src/tests/funcs");

	suite("funcs", []()
	{
		test({ "args_0_return_void", "args_0_return_explicit_void", "args_1_void_return_void" }, ROOT_PATH,
				[](syntax_tree& st)
				{
					const auto root = st.get_root_package();
					assert_equals(root->get_children().size(), 15);

					const auto project_module = assert_type<node_module>(root->get_child(13));
					assert_equals(project_module->get_name(), "westcoastcode.se/tests");

					const auto func = assert_type<node_func>(root->get_child(14));
					assert_equals(func->get_name(), "f");
					assert_not_null(func->get_body());
					assert_equals(func, func->get_body()->get_def());
					assert_not_null(func->get_arguments());
					assert_equals(func->get_arguments()->num_arguments(), 0);
					assert_not_null(func->get_returns());
					assert_equals(func->get_returns()->get_children().size(), 0);
				});
		test("args_0_return_float", ROOT_PATH, [](syntax_tree& st)
		{
			const auto root = st.get_root_package();
			assert_equals(root->get_children().size(), 15);

			const auto project_module = assert_type<node_module>(root->get_child(13));
			assert_equals(project_module->get_name(), "westcoastcode.se/tests");

			const auto func = assert_type<node_func>(root->get_children()[14]);
			assert_equals(func->get_name(), "f");
			assert_not_null(func->get_body());
			assert_equals(func, func->get_body()->get_def());
			assert_not_null(func->get_arguments());
			assert_equals(func->get_arguments()->num_arguments(), 0);
			const auto ret = assert_not_null(func->get_returns());
			assert_equals(ret->get_children().size(), 1);
			const auto ret_ref_float = assert_type<node_type_ref>(ret->get_child(0));
			assert_equals(ret_ref_float->get_size(), sizeof(float));
			assert_equals(ret_ref_float->get_type(), root->get_child(11));
			const auto ref_float = assert_type<node_ref>(ret_ref_float->get_child(0));
			assert_equals(ref_float->get_query_text(), "float");
			const auto body = assert_type<node_func_body>(func->get_child(2));
			assert_equals(body->get_children().size(), 1);
			assert_type<node_scope>(body->get_children()[0]);
		});
		test("args_0_return_ptr_int", ROOT_PATH, [](syntax_tree& st)
		{
			const auto root = st.get_root_package();
			assert_equals(root->get_children().size(), 15);

			const auto project_module = assert_type<node_module>(root->get_child(13));
			assert_equals(project_module->get_name(), "westcoastcode.se/tests");

			const auto func = assert_type<node_func>(root->get_child(14));
			assert_equals(func->get_name(), "f");
			assert_not_null(func->get_body());
			assert_equals(func, func->get_body()->get_def());
			assert_not_null(func->get_arguments());
			assert_equals(func->get_arguments()->num_arguments(), 0);
			const auto ret = assert_not_null(func->get_returns());
			assert_equals(ret->get_children().size(), 1);
			const auto ret_ptr_int = assert_type<node_type_accessor>(ret->get_child(0));
			const auto ret_ref_int = assert_type<node_type_ref>(ret_ptr_int->get_child(0));
			assert_equals(ret_ref_int->get_type(), root->get_child(7));
			const auto ref_int = assert_type<node_ref>(ret_ref_int->get_child(0));
			assert_equals(ref_int->get_query_text(), "int");
			const auto body = assert_type<node_func_body>(func->get_child(2));
			assert_equals(body->get_children().size(), 1);
			assert_type<node_scope>(body->get_children()[0]);
		});
		test("args_1_return_void", ROOT_PATH, [](syntax_tree& st)
		{
			const auto root = st.get_root_package();
			assert_equals(root->get_children().size(), 15);

			const auto project_module = assert_type<node_module>(root->get_child(13));
			assert_equals(project_module->get_name(), "westcoastcode.se/tests");

			const auto func = assert_type<node_func>(root->get_children()[14]);
			assert_equals(func->get_children().size(), 3);
			const auto args = assert_type<node_func_arguments>(func->get_child(0));
			const auto arg1 = assert_type<node_var>(args->get_child(0));
			assert_equals("a1", arg1->get_name());
			const auto arr_type_ref_int = assert_type<node_type_ref>(arg1->get_child(0));
			const auto arr_ref_int = assert_type<node_ref>(arr_type_ref_int->get_child(0));
			assert_equals(arr_ref_int->get_query_text(), "int");
			const auto body = assert_type<node_func_body>(func->get_child(2));
			assert_equals(body->get_children().size(), 1);
			assert_type<node_scope>(body->get_children()[0]);
		});
		test("args_1_array_return_void", ROOT_PATH, [](syntax_tree& st)
		{
			const auto root = st.get_root_package();
			assert_equals(root->get_children().size(), 15);

			const auto project_module = assert_type<node_module>(root->get_child(13));
			assert_equals(project_module->get_name(), "westcoastcode.se/tests");

			const auto func = assert_type<node_func>(root->get_children()[14]);
			assert_equals(func->get_children().size(), 3);
			const auto args = assert_type<node_func_arguments>(func->get_child(0));
			const auto arg1 = assert_type<node_var>(args->get_child(0));
			const auto arr = assert_type<node_type_array>(arg1->get_child(0));
			assert_equals(arr->get_children().size(), 2);
			const auto arr_const = assert_type<node_op_constant>(arr->get_child(0));
			assert_equals(arr_const->get_value().type, primitive_type::int32);
			assert_equals(arr_const->get_value().i32, 2);
			assert_equals(arr_const->get_children().size(), 1);
			const auto arr_type_ref_int = assert_type<node_type_ref>(arr->get_child(1));
			const auto arr_ref_int = assert_type<node_ref>(arr_type_ref_int->get_child(0));
			assert_equals(arr_ref_int->get_query_text(), "int");
		});
		test("args_1_anonymous_return_void", ROOT_PATH, [](syntax_tree& st)
		{
			const auto root = st.get_root_package();
			assert_equals(root->get_children().size(), 15);

			const auto project_module = assert_type<node_module>(root->get_child(13));
			assert_equals(project_module->get_name(), "westcoastcode.se/tests");

			const auto func = assert_type<node_func>(root->get_child(14));
			assert_equals(func->get_children().size(), 3);
			const auto args = assert_type<node_func_arguments>(func->get_child(0));
			const auto arg1 = assert_type<node_var>(args->get_child(0));
			assert_equals("_", arg1->get_name());
			const auto arr_type_ref_int = assert_type<node_type_ref>(arg1->get_child(0));
			assert_equals(arr_type_ref_int->get_type(), root->get_child(7));
			const auto arr_ref_int = assert_type<node_ref>(arr_type_ref_int->get_child(0));
			assert_equals(arr_ref_int->get_query_text(), "int");
			const auto body = assert_type<node_func_body>(func->get_child(2));
			assert_equals(body->get_children().size(), 1);
			assert_type<node_scope>(body->get_child(0));
		});
		test({ "inner_func", "inner_func_explicit_void" }, ROOT_PATH, [](syntax_tree& st)
		{
			const auto root = st.get_root_package();
			assert_equals(root->get_children().size(), 15);

			const auto func_f = assert_type<node_func>(root->get_child(14));
			assert_equals(func_f->get_children().size(), 3);
			const auto args_f = assert_type<node_func_arguments>(func_f->get_child(0));
			assert_equals(args_f->get_children().size(), 0);
			const auto body_f = assert_type<node_func_body>(func_f->get_child(2));
			assert_equals(body_f->get_children().size(), 1);
			const auto scope_f = assert_type<node_scope>(body_f->get_child(0));

			const auto func_f2 = assert_type<node_func>(scope_f->get_child(0));
			assert_equals(func_f2->get_children().size(), 3);
			const auto args_f2 = assert_type<node_func_arguments>(func_f2->get_child(0));
			assert_equals(args_f2->get_children().size(), 0);
			const auto body_f2 = assert_type<node_func_body>(func_f2->get_child(2));
			assert_equals(body_f2->get_children().size(), 1);
			const auto scope_f2 = assert_type<node_scope>(body_f2->get_child(0));
		});
		test("func_with_same_name_scopes", ROOT_PATH, [](syntax_tree& st)
		{
			const auto root = st.get_root_package();
			assert_equals(root->get_children().size(), 15);

			const auto func_f = assert_type<node_func>(root->get_child(14));
			assert_equals(func_f->get_name(), "f1");
			assert_equals(func_f->get_children().size(), 3);
			const auto args_f = assert_type<node_func_arguments>(func_f->get_child(0));
			assert_equals(args_f->get_children().size(), 0);
			const auto body_f = assert_type<node_func_body>(func_f->get_child(2));
			assert_equals(body_f->get_children().size(), 1);
			const auto scope_f = assert_type<node_scope>(body_f->get_child(0));
			const auto scope_f_scope = assert_type<node_scope>(scope_f->get_child(0));

			const auto func_f2_1 = assert_type<node_func>(scope_f_scope->get_child(0));
			assert_equals(func_f2_1->get_name(), "f2");
			assert_equals(func_f2_1->get_children().size(), 3);
			const auto args_f2_1 = assert_type<node_func_arguments>(func_f2_1->get_child(0));
			assert_equals(args_f2_1->get_children().size(), 0);
			const auto body_f2_1 = assert_type<node_func_body>(func_f2_1->get_child(2));
			assert_equals(body_f2_1->get_children().size(), 1);
			assert_type<node_scope>(body_f2_1->get_child(0));

			const auto func_f2_2 = assert_type<node_func>(scope_f->get_child(1));
			assert_equals(func_f2_2->get_name(), "f2");
			assert_equals(func_f2_2->get_children().size(), 3);
			const auto args_f2_2 = assert_type<node_func_arguments>(func_f2_2->get_child(0));
			assert_equals(args_f2_2->get_children().size(), 0);
			const auto body_f2_2 = assert_type<node_func_body>(func_f2_2->get_child(2));
			assert_equals(body_f2_2->get_children().size(), 1);
			assert_type<node_scope>(body_f2_2->get_child(0));
			assert_not_equals(func_f2_2, func_f2_1);
		});
		test("two_funcs_in_scope", ROOT_PATH, [](syntax_tree& st)
		{
			const auto root = st.get_root_package();
			assert_equals(root->get_children().size(), 15);

			const auto func_f = assert_type<node_func>(root->get_child(14));
			assert_equals(func_f->get_name(), "f1");
			assert_equals(func_f->get_children().size(), 3);
			const auto args_f = assert_type<node_func_arguments>(func_f->get_child(0));
			assert_equals(args_f->get_children().size(), 0);
			const auto body_f = assert_type<node_func_body>(func_f->get_child(2));
			assert_equals(body_f->get_children().size(), 1);
			const auto scope_f = assert_type<node_scope>(body_f->get_child(0));
			const auto scope_f_scope = assert_type<node_scope>(scope_f->get_child(0));

			const auto func_f2_1 = assert_type<node_func>(scope_f_scope->get_child(0));
			assert_equals(func_f2_1->get_name(), "f2_1");
			const auto func_f2_2 = assert_type<node_func>(scope_f_scope->get_child(1));
			assert_equals(func_f2_2->get_name(), "f2_2");

			const auto func_f1_1 = assert_type<node_func>(scope_f->get_child(1));
			assert_equals(func_f1_1->get_name(), "f1_1");
			const auto func_f1_2 = assert_type<node_func>(scope_f->get_child(2));
			assert_equals(func_f1_2->get_name(), "f1_2");
		});
		test("return_const", ROOT_PATH, [](syntax_tree& st)
		{
			const auto root = st.get_root_package();
			assert_equals(root->get_children().size(), 15);

			const auto func_f = assert_type<node_func>(root->get_child(14));
			assert_equals(func_f->get_name(), "f");
			assert_equals(func_f->get_children().size(), 3);
			const auto args_f = assert_type<node_func_arguments>(func_f->get_child(0));
			assert_equals(args_f->get_children().size(), 0);
			const auto body_f = assert_type<node_func_body>(func_f->get_child(2));
			assert_equals(body_f->get_children().size(), 1);
			const auto scope_f = assert_type<node_scope>(body_f->get_child(0));
			const auto func_f_ret = assert_type<node_op_return>(scope_f->get_child(0));
			const auto func_f_ret_const = assert_type<node_op_constant>(func_f_ret->get_child(0));
			assert_equals(func_f_ret_const->get_value().type, primitive_type::int32);
			assert_equals(func_f_ret_const->get_value().i32, 0);
		});
		test("return_expression", ROOT_PATH, [](syntax_tree& st)
		{
			const auto root = st.get_root_package();
			assert_equals(root->get_children().size(), 15);

			const auto func_f = assert_type<node_func>(root->get_child(14));
			assert_equals(func_f->get_name(), "f");
			assert_equals(func_f->get_children().size(), 3);
			const auto args_f = assert_type<node_func_arguments>(func_f->get_child(0));
			assert_equals(args_f->get_children().size(), 0);
			const auto body_f = assert_type<node_func_body>(func_f->get_child(2));
			assert_equals(body_f->get_children().size(), 1);
			const auto scope_f = assert_type<node_scope>(body_f->get_child(0));
			const auto func_f_ret = assert_type<node_op_return>(scope_f->get_child(0));
			const auto func_f_ret_const = assert_type<node_op_constant>(func_f_ret->get_child(0));
			assert_equals(func_f_ret_const->get_value().type, primitive_type::int32);
			assert_equals(func_f_ret_const->get_value().i32, 10 + 20 * 30);
		});
	});
}
