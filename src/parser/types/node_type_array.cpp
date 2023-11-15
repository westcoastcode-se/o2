//
// Part of the o2 Project, under the Apache License v2.0 with o2 Project Exceptions.
// See the LICENSE file in the project root for license terms
//

#include "node_type_array.h"
#include "../operations/node_op_constant.h"
#include <iostream>

using namespace o2;

node_type_array::node_type_array(const source_code_view& view)
		: node_type(view), _count()
{
}

node_type_array::node_type_array(const source_code_view& view, int count)
		: node_type(view), _count(count)
{

}

int node_type_array::resolve_size(const recursion_detector* rd)
{
	if (has_known_size())
		return _size;

	// Ensure that we aren't having any recursions
	rd->raise_error(this);
	assert(get_child_count() > 0);

	const recursion_detector rd0(rd, this);
	auto sub_type = dynamic_cast<node_type*>(get_child(0));
	return _count * sub_type->resolve_size(&rd0);
}

void node_type_array::debug(debug_ostream& stream, int indent) const
{
	stream << this << in(indent);
	stream << "type_array(count=" << _count << ")" << std::endl;
	node_type::debug(stream, indent);
}

bool node_type_array::resolve(const recursion_detector* rd)
{
	if (!node_type::resolve(rd))
		return false;

	if (_count > 0)
		return true;

	for (auto c: get_children())
	{
		const auto opc = dynamic_cast<node_op_constant*>(c);
		if (opc != nullptr)
		{
			if (opc->get_value().type >= primitive_type::uint64)
				throw std::runtime_error("constant value must be an non-decimal number");
			_count = opc->get_value().to_uint64();
			break;
		}
	}

	if (_count == 0)
		throw std::runtime_error("arrays must be a positive non-decimal non-zero number");
	return true;
}
