//
// Part of the o2 Project, under the Apache License v2.0 with o2 Project Exceptions.
// See the LICENSE file in the project root for license terms
//

#include "node_var_const.h"

using namespace o2;

void node_var_const::debug(std::basic_ostream<char>& stream, int indent) const
{
	stream << this << in(indent);
	stream << "var_const(name=" << _name << ")" << std::endl;
	node_symbol::debug(stream, indent);
}
