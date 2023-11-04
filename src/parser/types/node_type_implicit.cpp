//
// Part of the o2 Project, under the Apache License v2.0 with o2 Project Exceptions.
// See the LICENSE file in the project root for license terms
//

#include "node_type_implicit.h"
#include "../operations/node_op.h"

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

void node_type_implicit::debug(std::basic_ostream<char>& stream, int indent) const
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
	if (!node::resolve(rd))
		return false;

	auto op = dynamic_cast<node_op*>(get_child(0));
	if (op)
		_type = op->get_type();

	_type = _type->get_type();
	if (_type == nullptr)
		return false;
	return true;
}
