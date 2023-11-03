//
// Part of the o2 Project, under the Apache License v2.0 with o2 Project Exceptions.
// See the LICENSE file in the project root for license terms
//

#pragma once

#include "../primitive_value.h"

namespace o2
{

	/**
	 * \brief perform the decrement operator on a primitive value
	 * \param rhs the value on the right-hand side of the primitive value
	 * \return
	 */
	bool primitive_value_dec(primitive_value* rhs);
}