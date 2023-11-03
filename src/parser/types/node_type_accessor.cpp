//
// Part of the o2 Project, under the Apache License v2.0 with o2 Project Exceptions.
// See the LICENSE file in the project root for license terms
//

#include "node_type_accessor.h"
#include <iostream>

using namespace o2;

node_type_accessor::node_type_accessor(const source_code_view& view, accessor_type t)
		: node_type(view, sizeof(void*)), _accessor(t)
{
}

void node_type_accessor::debug(std::basic_ostream<char>& stream, int indent) const
{
	stream << this << in(indent);
	stream << "type_accessor(accessor=";
	if (_accessor == accessor_pointer)
		stream << "*";
	else
		stream << "&";
	stream << ")" << std::endl;
	node_type::debug(stream, indent);
}

bool node_type_accessor::resolve(const recursion_detector* rd)
{
	if (!node_type::resolve(rd))
		return false;
	return true;
}
