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

		});
	});
}
