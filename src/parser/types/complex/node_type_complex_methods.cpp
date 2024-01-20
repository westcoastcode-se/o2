//
// Part of the o2 Project, under the Apache License v2.0 with o2 Project Exceptions.
// See the LICENSE file in the project root for license terms
//

#include "node_type_complex_methods.h"

using namespace o2;

void node_type_complex_methods::debug(debug_ostream& stream, int indent) const
{
	stream << this << in(indent);
	stream << "type_struct_methods()" << std::endl;
	node::debug(stream, indent);
}
