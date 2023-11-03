//
// Part of the o2 Project, under the Apache License v2.0 with o2 Project Exceptions.
// See the LICENSE file in the project root for license terms
//

#include "node_type_implicit.h"

using namespace o2;

node_type_implicit::node_type_implicit(const source_code_view& view)
		: node_type(view), _type(this)
{
}

int node_type_implicit::resolve_size(const recursion_detector* rd)
{
	return 0;
}

void node_type_implicit::debug(std::basic_ostream<char>& stream, int indent) const
{
	stream << this << in(indent);
	stream << "type_implicit(type=";
	if (_type)
		stream << _type;
	else
		stream << "?";
	stream << ")" << std::endl;
	node_type::debug(stream, indent);
}

bool node_type_implicit::resolve(const recursion_detector* rd)
{
	if (!node::resolve(rd))
		return false;

	return true;
}
