//
// Part of the o2 Project, under the Apache License v2.0 with o2 Project Exceptions.
// See the LICENSE file in the project root for license terms
//

#pragma once

#include "../primitive_value.h"

namespace o2
{
	/**
	 * \param lhs the value we want to negate
	 * \return if we managed to negate the value
	 */
	bool primitive_value_neg(primitive_value* rhs);
}