//
// Part of the o2 Project, under the Apache License v2.0 with o2 Project Exceptions.
// See the LICENSE file in the project root for license terms
//

#include "node_var.h"
#include <iostream>

using namespace o2;

node_var::node_var(const source_code_view& view, string_view name)
		: node_symbol(view), _name(name), _type()
{

}

void node_var::debug(std::basic_ostream<char>& stream, int indent) const
{
	stream << this << in(indent);
	stream << "var(name=" << _name << ")" << std::endl;
	node_symbol::debug(stream, indent);
}

node* node_var::on_child_added(node* n)
{
	const auto nt = dynamic_cast<node_type*>(n);
	if (nt)
		_type = nt;
	return n;
}

void node_var::on_child_removed(node* n)
{
	if (_type == n)
		_type = nullptr;
}
