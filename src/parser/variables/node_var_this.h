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
		node_var_this(const source_code_view& view, string_view name, node_type* type)
				: node_var(view, name, modifier_readonly)
		{
			_type = type;
		}

#pragma region node

		void debug(std::basic_ostream<char>& stream, int indent) const final;

#pragma endregion

	};
}