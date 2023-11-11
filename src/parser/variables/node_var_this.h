//
// Part of the o2 Project, under the Apache License v2.0 with o2 Project Exceptions.
// See the LICENSE file in the project root for license terms
//

#pragma once

#include "node_var.h"

namespace o2
{
	/**
	 * \brief a specific type of variable that represents "this"
	 */
	class node_var_this
			: public node_var
	{
	public:
		node_var_this(const source_code_view& view, string_view name, int modifiers)
				: node_var(view, name, modifiers)
		{
		}

		node_var_this(const source_code_view& view, string_view name, int modifiers, string_view this_name)
				: node_var(view, name, modifiers), _this_name(this_name)
		{
		}

		/**
		 * \return the name for the "this" variable
		 */
		string_view get_this_name() const
		{
			return _this_name;
		}

	private:
		string_view _this_name;
	};
}