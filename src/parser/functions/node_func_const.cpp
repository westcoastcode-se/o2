//
// Part of the o2 Project, under the Apache License v2.0 with o2 Project Exceptions.
// See the LICENSE file in the project root for license terms
//

#include "node_func_const.h"

using namespace o2;

void node_func_const::resolve_symbol_id()
{
	const auto symbol = get_parent_of_type<node_symbol>();
	if (symbol)
	{
		std::stringstream ss;
		ss << symbol->get_id();
		ss << '.';
		ss << get_name();
		set_id(std::move(ss.str()));
	}
}

void node_func_const::debug(std::basic_ostream<char>& stream, int indent) const
{
	stream << this << in(indent);
	stream << "func_const(name=" << get_name() << ",body=" << get_body() << ")" << std::endl;
	node::debug(stream, indent);
}
