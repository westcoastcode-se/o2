//
// Part of the o2 Project, under the Apache License v2.0 with o2 Project Exceptions.
// See the LICENSE file in the project root for license terms
//

#include "node_scope.h"

using namespace o2;

void node_scope::debug(debug_ostream& stream, int indent) const
{
	stream << this << in(indent);
	stream << "scope()" << std::endl;
	node_symbol::debug(stream, indent);
}
