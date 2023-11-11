//
// Part of the o2 Project, under the Apache License v2.0 with o2 Project Exceptions.
// See the LICENSE file in the project root for license terms
//

#include "node_var_this.h"

using namespace o2;

void node_var_this::debug(std::basic_ostream<char>& stream, int indent) const
{
	stream << this << in(indent);
	stream << "var_this(name=" << _name << ",type=" << _type;
	if (bit_isset(_modifiers, modifier_readonly))
		stream << ",readonly";
	if (bit_isset(_modifiers, modifier_const))
		stream << ",const";
	stream << ")" << std::endl;
	node_symbol::debug(stream, indent);
}
