//
// Part of the o2 Project, under the Apache License v2.0 with o2 Project Exceptions.
// See the LICENSE file in the project root for license terms
//

#include "primitive_value_inc.h"

bool o2::primitive_value_inc(o2::primitive_value* rhs)
{
	switch (rhs->type)
	{
	case primitive_type::int8:
		++rhs->i8;
		break;
	case primitive_type::int16:
		++rhs->i16;
		break;
	case primitive_type::int32:
		++rhs->i32;
		break;
	case primitive_type::int64:
		++rhs->i64;
		break;
	case primitive_type::float32:
		++rhs->f32;
		break;
	case primitive_type::float64:
		++rhs->f64;
		break;
	case primitive_type::ptr:
		++rhs->ptr;
		break;
	default:
		return false;
	}
	return true;
}
