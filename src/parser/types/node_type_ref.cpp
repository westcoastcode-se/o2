//
// Part of the o2 Project, under the Apache License v2.0 with o2 Project Exceptions.
// See the LICENSE file in the project root for license terms
//

#include "node_type_ref.h"
#include <iostream>

using namespace o2;

node_type_ref::node_type_ref(const source_code_view& view)
		: node_type(view), _type(this)
{
}

void node_type_ref::set_ref(node_ref* ref)
{
	add_child(ref);
}

int node_type_ref::resolve_size(const recursion_detector* rd)
{
	if (has_known_size())
		return _size;

	rd->raise_error(this);
	if (_type == nullptr)
		throw resolve_error_unresolved_reference(get_source_code());

	const recursion_detector rd0(rd, this);
	_size = _type->resolve_size(&rd0);
	return _size;
}

void node_type_ref::debug(std::basic_ostream<char>& stream, int indent) const
{
	stream << this << in(indent);
	stream << "type_ref(type=";
	if (_type)
		stream << _type;
	else
		stream << "?";
	stream << ")" << std::endl;
	node_type::debug(stream, indent);
}

bool node_type_ref::resolve(const recursion_detector* rd)
{
	// resolve type first and then the rest of the children
	if (_type == this)
	{
		auto ref = dynamic_cast<node_ref*>(get_child(0));
		if (ref == nullptr)
			throw std::runtime_error("expected node_type_ref child-node to be a node_ref");
		if (!ref->resolve(rd))
			return false;
		const auto results = ref->get_result();
		if (results.size() > 1)
			throw resolve_error_multiple_refs(get_source_code());
		const auto type = dynamic_cast<node_type*>(results[0]);
		if (type == nullptr)
			return false;
		_type = type;
	}

	if (!node::resolve(rd))
		return false;

	_type = _type->get_type();
	if (_type == nullptr)
		return false;

	return true;
}
