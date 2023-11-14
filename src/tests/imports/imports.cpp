//
// Part of the o2 Project, under the Apache License v2.0 with o2 Project Exceptions.
// See the LICENSE file in the project root for license terms
//

#include "../utils.h"

using namespace std;
using namespace o2;

void imports()
{
	static const string_view ROOT_PATH("src/tests/imports");

	suite("imports", []()
	{
		test("one_local", ROOT_PATH, [](syntax_tree& st)
		{
			const auto root = st.get_root_package();
			assert_equals(root->get_children().size(), 15);

			const auto project_module = assert_type<module>(root->get_children()[13]);
			const auto package_models = assert_type<node_package>(project_module->get_children()[0]);
			assert_equals(package_models->get_name(), "/models");

			const auto import_models = assert_type<node_import>(root->get_children()[14]);
			assert_equals(import_models->get_children().size(), 1);
			const auto f = assert_type<node_func>(import_models->get_child(0));
			assert_equals(f->get_name(), "f");

		});
		test("two_local", ROOT_PATH, "apps", [](syntax_tree& st)
		{
			const auto root = st.get_root_package();
			assert_equals(root->get_children().size(), 15);

			const auto project_module = assert_type<module>(root->get_children()[13]);
			assert_equals(project_module->get_children().size(), 2);
			const auto package_models = assert_type<node_package>(project_module->get_children()[0]);
			assert_equals(package_models->get_name(), "/models");
			const auto package_services = assert_type<node_package>(project_module->get_children()[1]);
			assert_equals(package_services->get_name(), "/services");

			const auto import_models = assert_type<node_import>(root->get_children()[14]);
			const auto import_services = assert_type<node_import>(import_models->get_children()[0]);
			const auto f = assert_type<node_func>(import_services->get_children()[0]);
			assert_equals(f->get_name(), "f");
		});
	});
}
