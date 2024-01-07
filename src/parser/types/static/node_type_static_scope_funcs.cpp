//
// Part of the o2 Project, under the Apache License v2.0 with o2 Project Exceptions.
// See the LICENSE file in the project root for license terms
//

#include "node_type_static_scope_funcs.h"

using namespace o2;

void node_type_static_scope_funcs::debug(debug_ostream& stream, int indent) const
{
	stream << this << in(indent);
	stream << "type_static_scope_funcs()" << std::endl;
	node::debug(stream, indent);
}
