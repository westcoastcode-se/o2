//
// Part of the o2 Project, under the Apache License v2.0 with o2 Project Exceptions.
// See the LICENSE file in the project root for license terms
//

#include "node_type_implicit.h"
#include "../node_link.h"
#include "../operations/node_op_assign.h"

using namespace o2;

node_type_implicit::node_type_implicit(const source_code_view& view)
		: node_type(view), _type(this)
{
}

int node_type_implicit::resolve_size(const recursion_detector* rd)
{
	if (has_known_size())
		return _size;

	rd->raise_error(this);
	if (_type == this || _type == this)
		throw resolve_error_unresolved_reference(get_source_code());

	const recursion_detector rd0(rd, this);
	_size = _type->resolve_size(&rd0);
	return _size;
}

void node_type_implicit::debug(debug_ostream& stream, int indent) const
{
	stream << this << in(indent);
	stream << "type_implicit(type=";
	if (_type)
		stream << _type;
	else
		stream << "?";
	stream << ")" << std::endl;
	node_type::debug(stream, indent);
}

bool node_type_implicit::resolve(const recursion_detector* rd)
{
	if (_type == this)
	{
		const auto link = dynamic_cast<node_link*>(get_child(0));
		if (link == nullptr)
			throw expected_child_node(get_source_code(), "node_link");
		if (link->is_broken())
			throw expected_child_node(get_source_code(), "node_link->node_op");
		const auto op = dynamic_cast<node_op*>(link->get_node());
		if (op == nullptr)
			throw unexpected_child_node(get_source_code(), "node_link->node_op");

		const recursion_detector rd1(rd, this);
		if (!op->resolve(&rd1))
			return false;
		const auto type = op->get_type();
		if (type == nullptr)
			throw resolve_error_unresolved_reference(get_source_code());
		_type = type->get_type();
		if (_type == nullptr)
			return false;
	}

	if (!node::resolve(rd))
		return false;
	return true;
}

string node_type_implicit::get_id() const
{
	if (_type == this)
		return {};

	stringstream ss;
	const auto symbol = get_parent_of_type<node_symbol>();
	if (symbol)
		ss << symbol->get_id();
	ss << _type->get_id();
	ss << '.';
	return std::move(ss.str());
}
