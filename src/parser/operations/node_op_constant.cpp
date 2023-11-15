//
// Part of the o2 Project, under the Apache License v2.0 with o2 Project Exceptions.
// See the LICENSE file in the project root for license terms
//

#include "node_op_constant.h"
#include "../types/node_type.h"

using namespace o2;

void node_op_constant::debug(debug_ostream& stream, int indent) const
{
	stream << this << in(indent);
	stream << "constant(";
	switch (_value.type)
	{
	case primitive_type::unknown:
		stream << "unknown";
		break;
	case primitive_type::bool_:
		stream << "type=bool,";
		if (_value.bool_ == 1)
			stream << "value=true";
		else
			stream << "false";
		break;
	case primitive_type::int8:
		stream << "type=i8,";
		stream << _value.i8;
		break;
	case primitive_type::uint8:
		stream << "type=u8,";
		stream << _value.u8;
		break;
	case primitive_type::int16:
		stream << "type=i16,";
		stream << _value.i16;
		break;
	case primitive_type::uint16:
		stream << "type=u16,";
		stream << _value.u16;
		break;
	case primitive_type::int32:
		stream << "type=i32,";
		stream << _value.i32;
		break;
	case primitive_type::uint32:
		stream << "type=u32,";
		stream << _value.u32;
		break;
	case primitive_type::int64:
		stream << "type=i64,";
		stream << _value.i64;
		break;
	case primitive_type::uint64:
		stream << "type=u64,";
		stream << _value.u64;
		break;
	case primitive_type::float32:
		stream << "type=f32,";
		stream << _value.f32;
		break;
	case primitive_type::float64:
		stream << "type=f64,";
		stream << _value.f64;
		break;
	case primitive_type::ptr:
		stream << "type=ptr,";
		stream << static_cast<void*>(_value.ptr);
		break;
	default:
		stream << "type=?,value=?";
		break;
	}
	stream << ")" << std::endl;
	node_op::debug(stream, indent);
}

node* node_op_constant::on_child_added(node* n)
{
	const auto type = dynamic_cast<node_type*>(n);
	if (type)
		_type = type;
	return n;
}

void node_op_constant::on_child_removed(node* n)
{
	const auto type = dynamic_cast<node_type*>(n);
	if (type)
		_type = nullptr;
}
