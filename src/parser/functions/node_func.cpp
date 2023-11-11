//
// Part of the o2 Project, under the Apache License v2.0 with o2 Project Exceptions.
// See the LICENSE file in the project root for license terms
//

#include "node_func.h"
#include "node_func_body.h"
#include <iostream>

using namespace o2;

void node_func::resolve_symbol_id()
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

node* node_func::on_child_added(node* n)
{
	const auto arguments = dynamic_cast<node_func_arguments*>(n);
	if (arguments)
		_arguments = arguments;
	else
	{
		const auto returns = dynamic_cast<node_func_returns*>(n);
		if (returns)
			_returns = returns;
		else
		{
			const auto body = dynamic_cast<node_func_body*>(n);
			if (body)
				_body = body;
		}
	}
	return n;
}

void node_func::on_child_removed(node* n)
{
	if (_arguments == n)
		_arguments = nullptr;
	else if (_returns == n)
		_returns = nullptr;
	else if (_body == n)
		_body = nullptr;
}

void node_func::on_parent_node(node* p)
{
	resolve_symbol_id();
	test_collision();
}

void node_func::debug(std::basic_ostream<char>& stream, int indent) const
{
	stream << this << in(indent);
	stream << "func(name=" << _name << ",body=" << get_body() << ")" << std::endl;
	node::debug(stream, indent);
}
