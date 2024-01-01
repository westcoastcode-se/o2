//
// Part of the o2 Project, under the Apache License v2.0 with o2 Project Exceptions.
// See the LICENSE file in the project root for license terms
//

#include "resolve_state.h"
#include "package/node_package.h"

using namespace o2;

resolve_state::resolve_state(node_package* package)
		: _package(package)
{
}

void resolve_state::add_next_phase(node* n)
{
	_nodes.add(n);
}
