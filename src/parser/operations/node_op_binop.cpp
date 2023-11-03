//
// Part of the o2 Project, under the Apache License v2.0 with o2 Project Exceptions.
// See the LICENSE file in the project root for license terms
//

#include "node_op_binop.h"
#include "node_op_constant.h"
#include "../optimizations/primitive_value_compare.h"
#include "../optimizations/primitive_value_equals.h"
#include "../optimizations/primitive_value_not_equals.h"
#include "../optimizations/primitive_value_add.h"

using namespace o2;

node* node_op_binop::get_left() const
{
	const auto children = get_children();
	if (!children.empty())
		return children[0];
	return nullptr;
}

node* node_op_binop::get_right() const
{
	const auto children = get_children();
	if (children.size() > 1)
		return children[1];
	return nullptr;
}

void node_op_binop::debug(std::basic_ostream<char>& stream, int indent) const
{
	stream << this << in(indent);
	stream << "binop(op=";
	switch (_operator)
	{
	case minus:
		stream << "-";
		break;
	case plus:
		stream << "+";
		break;
	case mult:
		stream << "*";
		break;
	case div:
		stream << "/";
		break;
	case equals:
		stream << "==";
		break;
	case not_equals:
		stream << "!=";
		break;
	case less_then:
		stream << "<";
		break;
	case less_then_equals:
		stream << "<=";
		break;
	case greater_then:
		stream << ">";
		break;
	case greater_then_equals:
		stream << ">=";
		break;
	case bit_and:
		stream << "&";
		break;
	case bit_or:
		stream << "|";
		break;
	case bit_xor:
		stream << "^";
		break;
	default:
		stream << "unknown";
		break;
	}
	stream << ")" << std::endl;
	node_op::debug(stream, indent);
}

vector<node*> node_optimizer_binop_merge::optimize(node* n)
{
	const auto left = static_cast<node_op_binop*>(n)->get_left();
	const auto right = static_cast<node_op_binop*>(n)->get_right();

	if (dynamic_cast<node_op_constant*>(left) && dynamic_cast<node_op_constant*>(right))
	{
		const auto left_const = static_cast<node_op_constant*>(left);
		const auto right_const = static_cast<node_op_constant*>(right);
		auto left_value = left_const->get_value();
		const auto& right_value = right_const->get_value();

		typedef bool (* primitive_value_fn)(primitive_value*, const primitive_value*);
		primitive_value_fn fn = nullptr;
		switch (static_cast<node_op_binop*>(n)->get_operator())
		{
		case node_op_binop::minus:
			fn = primitive_value_sub;
			break;
		case node_op_binop::plus:
			fn = primitive_value_add;
			break;
		case node_op_binop::mult:
			fn = primitive_value_mult;
			break;
		case node_op_binop::div:
			fn = primitive_value_div;
			break;
		case node_op_binop::equals:
			fn = primitive_value_equals;
			break;
		case node_op_binop::not_equals:
			fn = primitive_value_not_equals;
			break;
		case node_op_binop::less_then:
			fn = primitive_value_less_then;
			break;
		case node_op_binop::less_then_equals:
			fn = primitive_value_less_equals;
			break;
		case node_op_binop::greater_then:
			break;
		case node_op_binop::greater_then_equals:
			break;
		case node_op_binop::bit_and:
			break;
		case node_op_binop::bit_or:
			break;
		case node_op_binop::bit_xor:
			break;
		case node_op_binop::unknown:
		default:
			return {};
		}

		if (fn && fn(&left_value, &right_value))
		{
			// TODO: Add support for converting the new type into the appropriate
			auto new_const = o2_new node_op_constant(n->get_source_code(), left_value);
			count++;
			return { new_const };
		}
	}
	return {};
}
