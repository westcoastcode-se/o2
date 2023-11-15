//
// Part of the o2 Project, under the Apache License v2.0 with o2 Project Exceptions.
// See the LICENSE file in the project root for license terms
//

#include "node_func_returns.h"

using namespace o2;

node_func_returns::node_func_returns(const source_code_view& view)
		: node(view)
{
	set_query_access_flags(query_access_modifier_passthrough);
}

void node_func_returns::debug(debug_ostream& stream, int indent) const
{
	stream << this << in(indent);
	stream << "returns()" << std::endl;
	node::debug(stream, indent);
}
