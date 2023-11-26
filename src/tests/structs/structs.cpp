//
// Part of the o2 Project, under the Apache License v2.0 with o2 Project Exceptions.
// See the LICENSE file in the project root for license terms
//

#include "../utils.h"

using namespace std;
using namespace o2;

void structs()
{
	static const string_view ROOT_PATH("src/tests/structs");

	suite("structs", []()
	{
		test({ "empty", "empty_with_no_body", "explicit_struct" }, ROOT_PATH, [](syntax_tree& st)
		{
			const auto root = st.get_root_package();
			assert_equals(root->get_children().size(), 14);

			const auto project_module = assert_type<node_module>(root->get_child(13));
			const auto package_main = assert_type<node_package>(project_module->get_child(0));

			const auto type_struct = assert_type<node_type_struct>(package_main->get_child(0));
			assert_equals(type_struct->get_name(), "empty");
			assert_equals(type_struct->get_child_count(), 0);
		});
		test("empty_in_module", ROOT_PATH, "apps", [](syntax_tree& st)
		{
			const auto root = st.get_root_package();
			assert_equals(root->get_children().size(), 14);

			const auto project_module = assert_type<node_module>(root->get_child(13));
			const auto package_main = assert_type<node_package>(project_module->get_child(0));

			const auto package_models = assert_type<node_package>(project_module->get_child(1));
			assert_equals(package_models->get_name(), "/models");

			const auto type_struct = assert_type<node_type_struct>(package_models->get_children()[0]);
			assert_equals(type_struct->get_name(), "empty");
			assert_equals(type_struct->get_child_count(), 0);
		});
		test("two_empty", ROOT_PATH, [](syntax_tree& st)
		{
			const auto root = st.get_root_package();
			assert_equals(root->get_children().size(), 14);

			const auto project_module = assert_type<node_module>(root->get_child(13));
			const auto package_main = assert_type<node_package>(project_module->get_child(0));

			const auto type_struct1 = assert_type<node_type_struct>(package_main->get_child(0));
			assert_equals(type_struct1->get_name(), "empty1");
			assert_equals(type_struct1->get_child_count(), 0);
			const auto type_struct2 = assert_type<node_type_struct>(package_main->get_child(1));
			assert_equals(type_struct2->get_name(), "empty2");
			assert_equals(type_struct2->get_child_count(), 0);
		});
		test("int_field", ROOT_PATH, [](syntax_tree& st)
		{
			const auto root = st.get_root_package();
			assert_equals(root->get_children().size(), 14);

			const auto project_module = assert_type<node_module>(root->get_child(13));
			const auto package_main = assert_type<node_package>(project_module->get_child(0));

			const auto type_struct1 = assert_type<node_type_struct>(package_main->get_child(0));
			assert_equals(type_struct1->get_name(), "empty");
			assert_equals(type_struct1->get_children().size(), 1);
			const auto fields = assert_type<node_type_struct_fields>(type_struct1->get_child(0));
			assert_equals(fields->get_children().size(), 1);
			const auto field1 = assert_type<node_type_struct_field>(fields->get_child(0));
			assert_equals(field1->get_name(), "first");
			const auto field1_type_ref = assert_type<node_type_ref>(field1->get_child(0));
			const auto field1_ref = assert_type<node_ref>(field1_type_ref->get_child(0));
			assert_equals(field1_ref->get_query_text(), "int");
			assert_equals(field1->get_field_type(), root->get_child(7));
		});
		test("int_float_fields", ROOT_PATH, [](syntax_tree& st)
		{
			const auto root = st.get_root_package();
			assert_equals(root->get_children().size(), 14);

			const auto project_module = assert_type<node_module>(root->get_child(13));
			const auto package_main = assert_type<node_package>(project_module->get_child(0));

			const auto type_struct1 = assert_type<node_type_struct>(package_main->get_child(0));
			assert_equals(type_struct1->get_name(), "empty");
			assert_equals(type_struct1->get_children().size(), 1);
			const auto fields = assert_type<node_type_struct_fields>(type_struct1->get_child(0));
			assert_equals(fields->get_children().size(), 2);

			const auto field1 = assert_type<node_type_struct_field>(fields->get_child(0));
			assert_equals(field1->get_name(), "first");
			assert_equals(field1->get_field_type(), root->get_child(7));
			const auto field1_type_ref = assert_type<node_type_ref>(field1->get_child(0));
			const auto field1_ref = assert_type<node_ref>(field1_type_ref->get_child(0));
			assert_equals(field1_ref->get_query_text(), "int");

			const auto field2 = assert_type<node_type_struct_field>(fields->get_child(1));
			assert_equals(field2->get_name(), "second");
			assert_equals(field2->get_field_type(), root->get_child(11));
			const auto field2_type_ref = assert_type<node_type_ref>(field2->get_child(0));
			const auto field2_ref = assert_type<node_ref>(field2_type_ref->get_child(0));
			assert_equals(field2_ref->get_query_text(), "float32");
		});
		test("type_inside_type", ROOT_PATH, [](syntax_tree& st)
		{
			const auto root = st.get_root_package();
			assert_equals(root->get_children().size(), 14);

			const auto project_module = assert_type<node_module>(root->get_child(13));
			const auto package_main = assert_type<node_package>(project_module->get_child(0));

			const auto type_struct_List = assert_type<node_type_struct>(package_main->get_child(0));
			assert_equals(type_struct_List->get_name(), "List");
			assert_equals(type_struct_List->get_child_count(), 1);

			const auto type_struct_node = assert_type<node_type_struct>(type_struct_List->get_child(0));
			assert_equals(type_struct_node->get_name(), "Node");
			assert_equals(type_struct_node->get_child_count(), 0);
		});
		test("ptr_type_field", ROOT_PATH, [](syntax_tree& st)
		{
			const auto root = st.get_root_package();
			assert_equals(root->get_children().size(), 14);

			const auto project_module = assert_type<node_module>(root->get_child(13));
			const auto package_main = assert_type<node_package>(project_module->get_child(0));

			const auto type_struct1 = assert_type<node_type_struct>(package_main->get_child(0));
			assert_equals(type_struct1->get_name(), "Node");
			assert_equals(type_struct1->get_children().size(), 1);
			const auto fields = assert_type<node_type_struct_fields>(type_struct1->get_child(0));
			assert_equals(fields->get_children().size(), 1);
			const auto field1 = assert_type<node_type_struct_field>(fields->get_child(0));
			assert_equals(field1->get_name(), "Head");
			const auto field1_ptr = assert_type<node_type_pointer_of>(field1->get_child(0));
			const auto field1_type_ref = assert_type<node_type_ref>(field1_ptr->get_child(0));
			const auto field1_ref = assert_type<node_ref>(field1_type_ref->get_child(0));
			assert_equals(field1_ref->get_query_text(), "Node");
			assert_equals(type_struct1->get_size(), sizeof(void*));
		});
		test("same_name_different_package", ROOT_PATH, [](syntax_tree& st)
		{
			const auto root = st.get_root_package();
			assert_equals(root->get_children().size(), 14);

			const auto project_module = assert_type<node_module>(root->get_child(13));
			assert_equals(project_module->get_child_count(), 3);
			const auto package_main = assert_type<node_package>(project_module->get_child(0));

			const auto package_services = assert_type<node_package>(project_module->get_child(1));
			assert_equals(package_services->get_children().size(), 1);
			const auto import_models = assert_type<node_import>(package_services->get_child(0));
			assert_equals(import_models->get_children().size(), 1);
			const auto services_empty = assert_type<node_type_struct>(import_models->get_child(0));
			assert_equals(services_empty->get_name(), "empty");
			assert_equals(services_empty->get_child_count(), 0);

			const auto package_models = assert_type<node_package>(project_module->get_child(2));
			assert_equals(package_models->get_children().size(), 1);
			const auto models_empty = assert_type<node_type_struct>(package_models->get_child(0));
			assert_equals(models_empty->get_name(), "empty");
			assert_equals(models_empty->get_child_count(), 0);
		});
		test("field_same_name_different_package", ROOT_PATH, [](syntax_tree& st)
		{
			const auto root = st.get_root_package();
			assert_equals(root->get_children().size(), 14);

			const auto project_module = assert_type<node_module>(root->get_child(13));
			assert_equals(project_module->get_child_count(), 3);
			const auto package_main = assert_type<node_package>(project_module->get_child(0));

			const auto package_services = assert_type<node_package>(project_module->get_child(1));
			assert_equals(package_services->get_children().size(), 1);
			const auto import_models = assert_type<node_import>(package_services->get_child(0));
			assert_equals(import_models->get_children().size(), 1);
			const auto services_empty = assert_type<node_type_struct>(import_models->get_child(0));
			assert_equals(services_empty->get_name(), "empty");
			assert_equals(services_empty->get_child_count(), 1);

			const auto package_models = assert_type<node_package>(project_module->get_child(2));
			assert_equals(package_models->get_children().size(), 1);
			const auto models_empty = assert_type<node_type_struct>(package_models->get_child(0));
			assert_equals(models_empty->get_name(), "empty");
			assert_equals(models_empty->get_child_count(), 0);

			const auto fields = assert_type<node_type_struct_fields>(services_empty->get_child(0));
			const auto field = assert_type<node_type_struct_field>(fields->get_child(0));
			assert_equals(field->get_field_type(), models_empty);
		});
		test("circular_package_fields", ROOT_PATH, [](syntax_tree& st)
		{
			const auto root = st.get_root_package();
			assert_equals(root->get_children().size(), 14);

			const auto project_module = assert_type<node_module>(root->get_child(13));
			assert_equals(project_module->get_child_count(), 3);
			const auto package_main = assert_type<node_package>(project_module->get_child(0));

			const auto package_services = assert_type<node_package>(project_module->get_child(1));
			assert_equals(package_services->get_children().size(), 1);
			const auto import_models = assert_type<node_import>(package_services->get_child(0));
			assert_equals(import_models->get_children().size(), 2);
			const auto services_s1 = assert_type<node_type_struct>(import_models->get_child(0));
			assert_equals(services_s1->get_name(), "s1");
			assert_equals(services_s1->get_children().size(), 1);
			const auto services_s2 = assert_type<node_type_struct>(import_models->get_child(1));
			assert_equals(services_s2->get_name(), "s2");
			assert_equals(services_s2->get_child_count(), 0);

			const auto package_models = assert_type<node_package>(project_module->get_child(2));
			assert_equals(package_models->get_children().size(), 1);
			const auto import_services = assert_type<node_import>(package_models->get_child(0));
			assert_equals(import_services->get_children().size(), 1);
			const auto models_m1 = assert_type<node_type_struct>(import_services->get_child(0));
			assert_equals(models_m1->get_name(), "m1");
			assert_equals(models_m1->get_children().size(), 1);

			const auto fields = assert_type<node_type_struct_fields>(services_s1->get_child(0));
			const auto field = assert_type<node_type_struct_field>(fields->get_child(0));
			assert_equals(field->get_field_type(), models_m1);
		});
		test({ "method_void", "method_explicit_this_void", "method_explicit_void",
			   "method_explicit_this_explicit_void" },
				ROOT_PATH, [](syntax_tree& st)
				{
					const auto root = st.get_root_package();
					assert_equals(root->get_children().size(), 14);

					const auto project_module = assert_type<node_module>(root->get_child(13));
					assert_equals(project_module->get_child_count(), 1);
					const auto package_main = assert_type<node_package>(project_module->get_child(0));

					const auto type_S = assert_type<node_type_struct>(package_main->get_child(0));
					assert_equals(type_S->get_name(), "S");
					assert_equals(type_S->get_child_count(), 1);
					assert_not_null(type_S->get_methods());
					assert_equals(type_S->get_methods()->get_child_count(), 1);

					const auto type_S_methods = assert_type<node_type_struct_methods>(type_S->get_child(0));
					const auto func_M = assert_type<node_func>(type_S_methods->get_child(0));
					assert_equals(func_M->get_name(), "M");
					assert_not_null(func_M->get_body());
					assert_equals(func_M, func_M->get_body()->get_def());
					assert_not_null(func_M->get_parameters());
					assert_equals(func_M->get_parameters()->num_arguments(), 1);
					assert_not_null(func_M->get_returns());
					assert_equals(func_M->get_returns()->get_children().size(), 0);

					const auto func_M_args = assert_type<node_func_parameters>(func_M->get_child(0));
					const auto func_M_arg1 = assert_type<node_var>(func_M_args->get_child(0));
					assert_equals("this", func_M_arg1->get_name());
					assert_equals(func_M_arg1->get_type(), type_S);
				});
		test("zero_static", ROOT_PATH, [](syntax_tree& st)
		{
			const auto root = st.get_root_package();
			assert_equals(root->get_children().size(), 14);

			const auto project_module = assert_type<node_module>(root->get_child(13));
			assert_equals(project_module->get_child_count(), 1);
			const auto package_main = assert_type<node_package>(project_module->get_child(0));
			const auto type_S = assert_type<node_type_struct>(package_main->get_child(0));
			assert_equals(type_S->get_name(), "S");
			assert_equals(type_S->get_child_count(), 1);
			assert_null(type_S->get_fields());
			assert_null(type_S->get_methods());
			assert_not_null(type_S->get_static());

			const auto type_S_static = assert_type<node_type_struct_static>(type_S->get_child(0));
			assert_equals(type_S_static->get_child_count(), 0);
		});
		test({ "static_int", "static_static_int" }, ROOT_PATH, [](syntax_tree& st)
		{
			const auto root = st.get_root_package();
			assert_equals(root->get_children().size(), 14);

			const auto project_module = assert_type<node_module>(root->get_child(13));
			assert_equals(project_module->get_child_count(), 1);
			const auto package_main = assert_type<node_package>(project_module->get_child(0));
			const auto type_S = assert_type<node_type_struct>(package_main->get_child(0));
			assert_equals(type_S->get_name(), "S");
			assert_equals(type_S->get_child_count(), 1);
			assert_null(type_S->get_fields());
			assert_null(type_S->get_methods());
			assert_not_null(type_S->get_static());

			const auto type_S_static = assert_type<node_type_struct_static>(type_S->get_child(0));
			assert_equals(type_S_static->get_child_count(), 1);

			const auto fields = assert_type<node_type_struct_static_vars>(type_S_static->get_child(0));
			assert_equals(fields->get_children().size(), 1);
			const auto field1 = assert_type<node_type_struct_field>(fields->get_child(0));
			assert_equals(field1->get_name(), "I");
			const auto field1_type_ref = assert_type<node_type_ref>(field1->get_child(0));
			const auto field1_ref = assert_type<node_ref>(field1_type_ref->get_child(0));
			assert_equals(field1_ref->get_query_text(), "int");
			assert_equals(field1->get_field_type(), root->get_child(7));
		});
		test({ "static_int_float", "static_int_static_float" }, ROOT_PATH, [](syntax_tree& st)
		{
			const auto root = st.get_root_package();
			assert_equals(root->get_children().size(), 14);

			const auto project_module = assert_type<node_module>(root->get_child(13));
			assert_equals(project_module->get_child_count(), 1);
			const auto package_main = assert_type<node_package>(project_module->get_child(0));
			const auto type_S = assert_type<node_type_struct>(package_main->get_child(0));
			assert_equals(type_S->get_name(), "S");
			assert_equals(type_S->get_child_count(), 1);
			assert_null(type_S->get_fields());
			assert_null(type_S->get_methods());
			assert_not_null(type_S->get_static());

			const auto type_S_static = assert_type<node_type_struct_static>(type_S->get_child(0));
			assert_equals(type_S_static->get_child_count(), 1);

			const auto fields = assert_type<node_type_struct_static_vars>(type_S_static->get_child(0));
			const auto field1 = assert_type<node_type_struct_field>(fields->get_child(0));
			assert_equals(field1->get_name(), "I");
			assert_equals(field1->get_field_type(), root->get_child(7));
			const auto field1_type_ref = assert_type<node_type_ref>(field1->get_child(0));
			const auto field1_ref = assert_type<node_ref>(field1_type_ref->get_child(0));
			assert_equals(field1_ref->get_query_text(), "int");

			const auto field2 = assert_type<node_type_struct_field>(fields->get_child(1));
			assert_equals(field2->get_name(), "F");
			assert_equals(field2->get_field_type(), root->get_child(11));
			const auto field2_type_ref = assert_type<node_type_ref>(field2->get_child(0));
			const auto field2_ref = assert_type<node_ref>(field2_type_ref->get_child(0));
			assert_equals(field2_ref->get_query_text(), "float");
		});
		test({ "static_func_void", "static_func_explicit_void" }, ROOT_PATH, [](syntax_tree& st)
		{
			const auto root = st.get_root_package();
			assert_equals(root->get_children().size(), 14);

			const auto project_module = assert_type<node_module>(root->get_child(13));
			assert_equals(project_module->get_child_count(), 1);
			const auto package_main = assert_type<node_package>(project_module->get_child(0));
			const auto type_S = assert_type<node_type_struct>(package_main->get_child(0));
			assert_equals(type_S->get_name(), "S");
			assert_equals(type_S->get_child_count(), 1);
			assert_null(type_S->get_fields());
			assert_null(type_S->get_methods());
			assert_not_null(type_S->get_static());

			const auto type_S_static = assert_type<node_type_struct_static>(type_S->get_child(0));
			assert_equals(type_S_static->get_child_count(), 1);

			const auto type_S_static_funcs = assert_type<node_type_struct_static_funcs>(type_S_static->get_child(0));
			const auto func_M = assert_type<node_func>(type_S_static_funcs->get_child(0));
			assert_equals(func_M->get_name(), "F");
			assert_not_null(func_M->get_body());
			assert_equals(func_M, func_M->get_body()->get_def());
			assert_not_null(func_M->get_parameters());
			assert_equals(func_M->get_parameters()->num_arguments(), 0);
			assert_not_null(func_M->get_returns());
			assert_equals(func_M->get_returns()->get_children().size(), 0);
		});
	});
}