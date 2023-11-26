//
// Part of the o2 Project, under the Apache License v2.0 with o2 Project Exceptions.
// See the LICENSE file in the project root for license terms
//

#include "node_type_array.h"
#include "../operations/node_op_constant.h"
#include <iostream>

using namespace o2;

node_type_array::node_type_array(const source_code_view& view)
		: node_type(view), _count(-1), _array_type(this), _operation()
{
}

node_type_array::node_type_array(const source_code_view& view, int count)
		: node_type(view), _count(count), _array_type(this), _operation()
{

}

int node_type_array::resolve_size(const recursion_detector* rd)
{
	if (has_known_size())
		return _size;

	// Ensure that we aren't having any recursions
	rd->raise_error(this);
	const recursion_detector rd0(rd, this);
	return _count * _array_type->resolve_size(&rd0);
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

	// arrays must have a type associated with it
	if (_array_type == nullptr)
		throw expected_child_node(get_source_code(), "node_type");

	// the array has been resolved already
	if (_count != -1)
		return true;

	// if no count is hard-coded then an operation must be associated with the array
	if (_operation == nullptr)
		throw expected_child_node(get_source_code(), "node_op");

	// The operation can be a lot of things
	// TODO: Maybe force the use of a "node_op_expression" which evaluates into a value.
	//       Force the use of node_op_constant for now
	const auto opc = dynamic_cast<node_op_constant*>(_operation);
	if (opc == nullptr)
		throw expected_child_node(get_source_code(), "node_op_constant");

	if (opc->get_value().type >= primitive_type::uint64)
		throw std::runtime_error("constant value must be an non-decimal number");
	_count = (int)opc->get_value().to_uint64();

	if (_count == 0)
		throw resolve_error_positive_int_constant_expected(get_source_code());
	return true;
}

string node_type_array::get_id() const
{
	assert(_count != -1);

	stringstream ss;
	ss << '[' << _count << ']';
	ss << _array_type->get_id();
	return std::move(ss.str());
}

node* node_type_array::on_child_added(node* n)
{
	auto type = dynamic_cast<node_type*>(n);
	if (type != nullptr)
		_array_type = type;
	else
	{
		const auto op = dynamic_cast<node_op*>(n);
		if (op)
			_operation = op;
	}
	return n;
}

void node_type_array::on_child_removed(node* n)
{
	if (n == _array_type)
		n = nullptr;
	else if (n == _operation)
		_operation = nullptr;
}
