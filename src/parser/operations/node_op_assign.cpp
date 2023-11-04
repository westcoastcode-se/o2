//
// Part of the o2 Project, under the Apache License v2.0 with o2 Project Exceptions.
// See the LICENSE file in the project root for license terms
//

#include "node_op_assign.h"

using namespace o2;

node_op_assign::node_op_assign(const source_code_view& view)
		: node_op(view), _variable(), _expression()
{
}

node_type* node_op_assign::get_type()
{
	const auto op = dynamic_cast<node_op*>(get_child(0));
	if (op)
		return op->get_type();
	return nullptr;
}

void node_op_assign::debug(std::basic_ostream<char>& stream, int indent) const
{
	stream << this << in(indent);
	stream << "op_assign()" << std::endl;
	node::debug(stream, indent);
}

node* node_op_assign::on_child_added(node* n)
{
	const auto var = dynamic_cast<node_var*>(n);
	if (var)
	{
		_variable = var;
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
	if (n == _variable)
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

bool node_op_assign::resolve(const recursion_detector* rd)
{
	if (!node::resolve(rd))
		return false;

	if (_variable == nullptr)
	{
		// assume that
		const auto ref = dynamic_cast<node_ref*>(get_child(0));
		if (ref == nullptr)
			throw expected_child_node(get_source_code(), "node_ref");
	}

	return true;
}

void node_op_assign::on_parent_node(node* p)
{
	if (_expression == nullptr)
		throw expected_child_node(get_source_code(), "node_op");

	node::on_parent_node(p);
}
