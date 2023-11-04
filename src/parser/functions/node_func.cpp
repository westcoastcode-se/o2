//
// Part of the o2 Project, under the Apache License v2.0 with o2 Project Exceptions.
// See the LICENSE file in the project root for license terms
//

#include "node_func.h"
#include <cassert>
#include <iostream>

using namespace o2;

void node_func_def::debug(std::basic_ostream<char>& stream, int indent) const
{
	stream << this << in(indent);
	stream << "func_def(name=" << _name << ", modifiers=[";
	for (int i = 0; i < 4; ++i)
	{
		const int bit = (1 << i);
		if ((bit & _modifiers))
		{
			switch (static_cast<modifiers>(bit))
			{
			case modifier_extern:
				stream << "extern,";
				break;
			case modifier_const:
				stream << "const,";
				break;
			case modifier_macro:
				stream << "macro,";
				break;
			case modifier_inner_func:
				stream << "inner_func,";
				break;
			default:
				stream << "?,";
			}
		}
	}
	stream << "], body=" << _body << ")" << std::endl;
	node::debug(stream, indent);
}

node* node_func_def::on_child_added(node* n)
{
	const auto arguments = dynamic_cast<node_func_arguments*>(n);
	if (arguments)
		_arguments = arguments;
	const auto returns = dynamic_cast<node_func_returns*>(n);
	if (returns)
		_returns = returns;
	const auto body = dynamic_cast<node_func_body*>(n);
	if (body)
	{
		_body = body;
		_body->set_def(this);
	}
	return n;
}

void node_func_def::on_child_removed(node* n)
{
	if (_arguments == n)
		_arguments = nullptr;
	else if (_returns == n)
		_returns = nullptr;
	else if (_body == n)
	{
		_body->set_def(nullptr);
		_body = nullptr;
	}
}

void node_func_def::resolve_symbol_id()
{
	const auto symbol = get_parent_of_type<node_symbol>();
	if (symbol)
	{
		std::stringstream ss;
		ss << symbol->get_id();
		ss << '.';
		ss << _name;
		set_id(std::move(ss.str()));
	}
}

void node_func_def::on_parent_node(node* p)
{
	resolve_symbol_id();
	test_collision();
}

void node_func_body::debug(std::basic_ostream<char>& stream, int indent) const
{
	stream << this << in(indent);
	stream << "func_body(def=" << _def << ")" << std::endl;
	node::debug(stream, indent);
}
