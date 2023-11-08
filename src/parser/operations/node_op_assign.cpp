//
// Part of the o2 Project, under the Apache License v2.0 with o2 Project Exceptions.
// See the LICENSE file in the project root for license terms
//

#include "node_op_assign.h"
#include "../types/node_type_implicit.h"
#include "../node_link.h"

using namespace o2;

node_op_assign::node_op_assign(const source_code_view& view)
		: node_op(view), _variable(), _expression()
{
}

node_type* node_op_assign::get_type()
{
	return _variable->get_type();
}

void node_op_assign::debug(std::basic_ostream<char>& stream, int indent) const
{
	stream << this << in(indent);
	stream << "op_assign()" << std::endl;
	node::debug(stream, indent);
}

node* node_op_assign::on_child_added(node* n)
{
	const auto link = dynamic_cast<node_link*>(n);
	if (link && !link->is_broken())
	{
		_variable = dynamic_cast<node_var*>(link->get_node());
		return n;
	}

	const auto op = dynamic_cast<node_op*>(n);
	if (op)
	{
		_expression = op;
		return n;
	}

	return node::on_child_added(n);
}

void node_op_assign::on_child_removed(node* n)
{
	const auto link = dynamic_cast<node_link*>(n);
	if (link && link->get_node() == _variable)
	{
		_variable = nullptr;
		return;
	}

	if (n == _expression)
	{
		_expression = nullptr;
		return;
	}

	node::on_child_removed(n);
}
