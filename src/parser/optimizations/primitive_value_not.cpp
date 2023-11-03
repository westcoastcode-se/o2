//
// Part of the o2 Project, under the Apache License v2.0 with o2 Project Exceptions.
// See the LICENSE file in the project root for license terms
//

#include "primitive_value_not.h"

bool o2::primitive_value_not(o2::primitive_value* const rhs)
{
	switch (rhs->type)
	{
	case primitive_type::float32:
	case primitive_type::float64:
	case primitive_type::ptr:
		return false;
	default:
		rhs->i32 = !rhs->i32;
		return true;
	}
}
