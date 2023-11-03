//
// Part of the o2 Project, under the Apache License v2.0 with o2 Project Exceptions.
// See the LICENSE file in the project root for license terms
//

#include "../utils.h"

using namespace std;
using namespace o2;

void const_()
{

	static const string_view ROOT_PATH("src/tests/const");

	suite("const", []()
	{
		test("const_global", ROOT_PATH, [](syntax_tree& st)
		{
			debug(st);
		});
	});
}
