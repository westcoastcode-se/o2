//
// Part of the o2 Project, under the Apache License v2.0 with o2 Project Exceptions.
// See the LICENSE file in the project root for license terms
//

#pragma once

#include "node_var.h"

namespace o2
{
	/**
	 * \brief a constant variable
	 */
	class node_var_const
			: public node_var
	{
	public:
		node_var_const(const source_code_view& view, string_view name)
				: node_var(view, name)
		{
		}
	};
}