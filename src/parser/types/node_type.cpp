//
// Part of the o2 Project, under the Apache License v2.0 with o2 Project Exceptions.
// See the LICENSE file in the project root for license terms
//

#include "node_type.h"

using namespace o2;

node_type::node_type(const source_code_view& view)
		: node_type(view, -1)
{
}

node_type::node_type(const source_code_view& view, int size)
		: node_symbol(view), _size(size)
{
}
