//
// Part of the o2 Project, under the Apache License v2.0 with o2 Project Exceptions.
// See the LICENSE file in the project root for license terms
//

#include "primitive_value_bit_not.h"

bool o2::primitive_value_bit_not(o2::primitive_value* const rhs)
{
	switch (rhs->type)
	{
	case primitive_type::int8:
		rhs->i32 = ~rhs->i8;
		rhs->type = primitive_type::int32;
		break;
	case primitive_type::uint8:
		rhs->u32 = ~rhs->u8;
		rhs->type = primitive_type::uint32;
		break;
	case primitive_type::int16:
		rhs->i32 = ~rhs->i16;
		rhs->type = primitive_type::int32;
		break;
	case primitive_type::uint16:
		rhs->u32 = ~rhs->u16;
		rhs->type = primitive_type::uint32;
		break;
	case primitive_type::int32:
		rhs->i32 = ~rhs->i32;
		break;
	case primitive_type::uint32:
		rhs->u32 = ~rhs->u32;
		break;
	case primitive_type::int64:
		rhs->i64 = ~rhs->i64;
		break;
	case primitive_type::uint64:
		rhs->u64 = ~rhs->u64;
		break;
	default:
		return false;
	}
	return true;
}
