//
// Part of the o2 Project, under the Apache License v2.0 with o2 Project Exceptions.
// See the LICENSE file in the project root for license terms
//

#include "node_named_variable.h"
#include <iostream>

using namespace o2;

node_named_variable::node_named_variable(const source_code_view& view, string_view name, int modifiers)
		: node_symbol(view), _name(name), _type(), _modifiers(modifiers)
{

}

void node_named_variable::debug(std::basic_ostream<char>& stream, int indent) const
{
	stream << this << in(indent);
	stream << "named_variable(name=" << _name << ",modifiers=[";
	for (int i = 0; i < 1; ++i)
	{
		const int bit = (1 << i);
		if ((bit & _modifiers))
		{
			switch (static_cast<modifiers>(bit))
			{
			case modifier_const:
				stream << "const,";
				break;
			default:
				stream << "?,";
			}
		}
	}
	stream << "])" << std::endl;
	node_symbol::debug(stream, indent);
}

node* node_named_variable::on_child_added(node* n)
{
	const auto nt = dynamic_cast<node_type*>(n);
	if (nt)
		_type = nt;
	return n;
}

void node_named_variable::on_child_removed(node* n)
{
	if (_type == n)
		_type = nullptr;
}
