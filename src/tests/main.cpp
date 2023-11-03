//
// Part of the o2 Project, under the Apache License v2.0 with o2 Project Exceptions.
// See the LICENSE file in the project root for license terms
//

#include "test.h"

extern void packages();

extern void funcs();

extern void imports();

extern void modules();

extern void structs();

extern void errors_parse();

extern void errors_resolve();

extern void const_();

int main(int argc, char** argv)
{
	o2::testing::test_state::stop_suit_on_error() = true;
	o2::testing::test_state::debugging() = true;

	std::string ignore;
	if (argc > 1)
		ignore = argv[1];
	if (argc > 2)
	{
		ignore += '.';
		ignore += argv[2];
	}
	if (!ignore.empty())
		o2::testing::test_state::test_name() = ignore;

	packages();
	imports();
	modules();
	funcs();
	structs();
	errors_parse();
	errors_resolve();
	const_();

	return o2::testing::test_state::success() ? 0 : 1;
}
