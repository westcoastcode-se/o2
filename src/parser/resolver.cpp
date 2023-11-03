//
// Part of the o2 Project, under the Apache License v2.0 with o2 Project Exceptions.
// See the LICENSE file in the project root for license terms
//

#include "resolver.h"

using namespace o2;

void o2::resolve(syntax_tree* const st, parser_state* const ps)
{
	st->resolve();

	// Resolve sizes
	auto nodes = ps->get_resolve_size_nodes();
	if (!nodes.empty())
	{
		const recursion_detector rd;
		for (auto node: nodes)
		{
			node->resolve_size(&rd);
		}
	}
}
