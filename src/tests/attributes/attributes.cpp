//
// Part of the o2 Project, under the Apache License v2.0 with o2 Project Exceptions.
// See the LICENSE file in the project root for license terms
//

#include "../utils.h"

using namespace std;
using namespace o2;

void attributes()
{
	static const string_view ROOT_PATH("src/tests/attributes");

	suite("attributes", []()
	{
		test("one_on_function", ROOT_PATH, [](syntax_tree& st)
		{
			const auto root = st.get_root_package();
			assert_equals(root->get_child_count(), 15);

			const auto project_module = assert_type<node_module>(root->get_child(13));
			assert_equals(project_module->get_name(), "westcoastcode.se/tests");
			const auto package_main = assert_type<node_package>(project_module->get_child(0));

			assert_equals(package_main->get_child_count(), 1);
			const auto import_stdlib = assert_type<node_import>(package_main->get_child(0));
			assert_equals(import_stdlib->get_child_count(), 3);

			const auto type_M = assert_type<node_type_struct>(import_stdlib->get_child(0));

			const auto func_F1 = assert_type<node_func>(import_stdlib->get_child(1));
			assert_equals(func_F1->get_name(), "F1");
			const auto F1_attributes = assert_not_null(func_F1->get_attributes());
			assert_equals(F1_attributes->get_child_count(), 1);
			const auto F1_attribute = assert_type<node_attribute>(F1_attributes->get_child(0));
			assert_equals(F1_attribute->get_attribute_type(), type_M);

			const auto func_F2 = assert_type<node_func>(import_stdlib->get_child(2));
			assert_equals(func_F2->get_name(), "F2");
			const auto F2_attributes = assert_not_null(func_F2->get_attributes());
			assert_equals(F2_attributes->get_child_count(), 1);
			const auto F2_attribute = assert_type<node_attribute>(F2_attributes->get_child(0));
			assert_equals(F2_attribute->get_attribute_type(), type_M);

			const auto project_stdlib = assert_type<node_module>(root->get_child(14));
			assert_equals(project_stdlib->get_name(), "stdlib");
			const auto package_stdlib = assert_type<node_package>(project_stdlib->get_child(0));
			const auto attribute = assert_type<node_type_struct>(package_stdlib->get_child(0));
			assert_equals(attribute->get_name(), "attribute");

			assert_true(type_M->inherits_from_type(attribute));
		});
	});
}
