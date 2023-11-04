//
// Part of the o2 Project, under the Apache License v2.0 with o2 Project Exceptions.
// See the LICENSE file in the project root for license terms
//

#include "node_op_unaryop.h"
#include "node_op_constant.h"
#include "../optimizations/primitive_value_not.h"
#include "../optimizations/primitive_value_bit_not.h"
#include "../optimizations/primitive_value_dec.h"
#include "../optimizations/primitive_value_inc.h"
#include "../optimizations/primitive_value_neg.h"

using namespace o2;

void node_op_unaryop::debug(std::basic_ostream<char>& stream, int indent) const
{
	stream << this << in(indent);
	stream << "unaryop(op=";
	switch (_operator)
	{
	case minus:
		stream << "-";
		break;
	case plus:
		stream << "+";
		break;
	case inc:
		stream << "++";
		break;
	case dec:
		stream << "--";
		break;
	case bit_not:
		stream << "~";
		break;
	case not_:
		stream << "!";
		break;
	default:
		stream << "unknown";
		break;
	}
	stream << ")" << std::endl;
	node_op::debug(stream, indent);
}

node* node_op_unaryop::get_right() const
{
	const auto children = get_children();
	if (!children.empty())
		return children[0];
	return nullptr;
}

void node_op_unaryop::on_parent_node(node* n)
{
	if (get_children().size() != 1)
		throw expected_child_node(get_source_code(), "one node_op child is expected");
}

vector<node*> node_optimizer_unaryop_merge::optimize(node* n)
{
	const auto right = static_cast<node_op_unaryop*>(n)->get_right();

	if (dynamic_cast<node_op_constant*>(right))
	{
		const auto right_const = static_cast<node_op_constant*>(right);
		auto value = right_const->get_value();
		switch (static_cast<node_op_unaryop*>(n)->get_operator())
		{
		case node_op_unaryop::minus:
			if (primitive_value_neg(&value))
			{
				count++;
				return o2_new node_op_constant(n->get_source_code(), value);
			}
			else
				break;
		case node_op_unaryop::plus:
			return { right };
		case node_op_unaryop::inc:
			if (primitive_value_inc(&value))
			{
				count++;
				return o2_new node_op_constant(n->get_source_code(), value);
			}
			break;
		case node_op_unaryop::dec:
			if (primitive_value_dec(&value))
			{
				count++;
				return o2_new node_op_constant(n->get_source_code(), value);
			}
			break;
		case node_op_unaryop::bit_not:
			if (primitive_value_bit_not(&value))
			{
				count++;
				return o2_new node_op_constant(n->get_source_code(), value);
			}
			break;
		case node_op_unaryop::not_:
			if (primitive_value_not(&value))
			{
				count++;
				return o2_new node_op_constant(n->get_source_code(), value);
			}
			break;
		case node_op_unaryop::unknown:
			break;
		}
	}
	return {};
}
