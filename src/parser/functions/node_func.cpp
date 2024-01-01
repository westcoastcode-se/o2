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
	const auto arguments = dynamic_cast<node_func_parameters*>(n);
	if (arguments)
		_parameters = arguments;
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
	if (_parameters == n)
		_parameters = nullptr;
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
	node_symbol::write_json_properties(j);
	j.write(json::pair<string_view>{ "name", get_name() });

	if (_parameters)
	{
		auto args = j.write(json::array{ "params" });
		for (auto a: _parameters->get_parameters())
		{
			a->write_json(args);
		}
	}

	if (_returns)
	{
		auto rets = j.write(json::array{ "returns" });
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
	if (_parameters)
	{
		const auto children = _parameters->get_parameters();
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
	if (get_name() != rhs->get_name())
		return false;

	const auto has_parameters1 = _parameters != nullptr;
	const auto has_parameters2 = rhs->_parameters != nullptr;
	if (has_parameters1 != has_parameters2)
		return false;

	const auto num_children1 = _parameters != nullptr ? _parameters->get_child_count() : 0;
	const auto num_children2 = rhs->_parameters != nullptr ? rhs->_parameters->get_child_count() : 0;
	if (num_children1 != num_children2)
		return false;

	// TODO: verify each parameter
	// TODO: varargs parameters?
	return true;
}
