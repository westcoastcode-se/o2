//
// Part of the o2 Project, under the Apache License v2.0 with o2 Project Exceptions.
// See the LICENSE file in the project root for license terms
//

#include "node_func.h"
#include "node_func_body.h"
#include <iostream>

using namespace o2;

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
	test_collision(this);
}

void node_func::debug(debug_ostream& stream, int indent) const
{
	stream << this << in(indent);
	stream << "func(name=" << get_name() << ",body=" << get_body();
	if (is_const())
		stream << ",const";
	stream << ")" << std::endl;
	node::debug(stream, indent);
}

void node_func::write_json_properties(json& j)
{
	j.write(json::pair<string_view>{ "type", "func" });
	node_symbol::write_json_properties(j);
	j.write(json::pair<string_view>{ "name", get_name() });
	if (_arguments)
	{
		auto args = j.write(json::array{ "args" });
		for (auto a: _arguments->get_arguments())
		{
			a->write_json(args);
		}
	}
	if (_returns)
	{
		auto rets = j.write(json::array{ "rets" });
		for (auto r: _returns->get_children())
		{
			r->write_json(rets);
		}
	}
}

string node_func::get_id() const
{
	stringstream ss;
	const auto symbol = get_parent_of_type<node_symbol>();
	string id;
	if (symbol)
		id = symbol->get_id();
	ss << id;
	if (!id.ends_with('/'))
		ss << '/';
	ss << get_name();
	ss << '(';
	if (_arguments)
	{
		const auto children = _arguments->get_arguments();
		for (int i = 0; i < children.size(); ++i)
		{
			const auto type = children[i]->get_type();
			if (i > 0)
				ss << ',';
			ss << type->get_id();
		}
	}
	ss << ')';
	ss << '(';
	if (_returns)
	{
		const auto children = _returns->get_children();
		for (int i = 0; i < children.size(); ++i)
		{
			const auto type = dynamic_cast<node_type*>(children[i]);
			if (i > 0)
				ss << ',';
			ss << type->get_id();
		}
	}
	ss << ')';
	return std::move(ss.str());
}

bool node_func::compare_with_symbol(const node_func* rhs) const
{
	return get_name() == rhs->get_name();
}
