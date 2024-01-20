//
// Part of the o2 Project, under the Apache License v2.0 with o2 Project Exceptions.
// See the LICENSE file in the project root for license terms
//

#include "../utils.h"

using namespace std;
using namespace o2;

void interfaces()
{
	static const string_view ROOT_PATH("src/tests/interfaces");

	suite("interfaces", []()
	{
		test({ "explicit_empty" }, ROOT_PATH, [](syntax_tree& st)
		{
			const auto root = st.get_root_package();
			assert_equals(root->get_children().size(), 14);

			const auto project_module = assert_type<node_module>(root->get_child(13));
			const auto package_main = assert_type<node_package>(project_module->get_child(0));

			const auto type_interface = assert_type<node_type_complex>(package_main->get_child(0));
			assert_equals(type_interface->get_name(), "I");
			assert_equals(type_interface->get_child_count(), 1);
			assert_true(type_interface->is_interface());
		});
	});
}