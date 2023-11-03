//
// Part of the o2 Project, under the Apache License v2.0 with o2 Project Exceptions.
// See the LICENSE file in the project root for license terms
//

#include "primitive_value_not_equals.h"

bool o2::primitive_value_not_equals(o2::primitive_value* lhs,
		const o2::primitive_value* rhs)
{
	// if both are pointers then make sure that both are
	if (lhs->type == primitive_type::ptr || rhs->type == primitive_type::ptr)
	{
		if (rhs->type != lhs->type)
			return false;
	}
	lhs->bool_ = lhs->i64 != rhs->i64;
	lhs->type = primitive_type::bool_;
	return true;
}
