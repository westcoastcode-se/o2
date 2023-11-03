//
// Part of the o2 Project, under the Apache License v2.0 with o2 Project Exceptions.
// See the LICENSE file in the project root for license terms
//

#pragma once

#include "../primitive_value.h"

namespace o2
{
	/**
	 * \brief not equals operator between the two supplied values
	 * \param lhs
	 * \param rhs
	 * \return
	 */
	bool primitive_value_not_equals(primitive_value* lhs, const primitive_value* rhs);
}