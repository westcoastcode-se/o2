//
// Part of the o2 Project, under the Apache License v2.0 with o2 Project Exceptions.
// See the LICENSE file in the project root for license terms
//

#include "../utils.h"

using namespace std;
using namespace o2;

void packages()
{
	static const string_view ROOT_PATH("src/tests/packages");

	suite("packages", []()
	{
		test("empty", ROOT_PATH, [](syntax_tree& st)
		{
			// verify that the number of items in the root package is known and pre-loaded
			const auto root = st.get_root_package();
			assert_equals(root->get_children().size(), 14);
		});
	});
}
