//
// Part of the o2 Project, under the Apache License v2.0 with o2 Project Exceptions.
// See the LICENSE file in the project root for license terms
//

#include "node_type_known_ref.h"

using namespace o2;

node_type_known_ref::node_type_known_ref(const source_code_view& view, node_type_primitive* const primitive)
		: node_type(view, primitive->get_size()), _type(primitive)
{
}

void node_type_known_ref::debug(debug_ostream& stream, int indent) const
{
	stream << this << in(indent);
	stream << "known_ref(type=";
	stream << _type;
	stream << ")" << std::endl;
	node_type::debug(stream, indent);
}
