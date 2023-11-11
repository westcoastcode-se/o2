//
// Part of the o2 Project, under the Apache License v2.0 with o2 Project Exceptions.
// See the LICENSE file in the project root for license terms
//

#pragma once

#include "node_func.h"

namespace o2
{
	/**
	 * \brief a constant function
	 *
	 * this type of function is allowed to be compile-time evaluated and thus is only allowed to have
	 * types and operations that are constant
	 */
	class node_func_const
			: public node_func
	{
	public:
		node_func_const(const source_code_view& view, string_view name)
				: node_func(view, name)
		{
		}

#pragma region node_symbol

		void resolve_symbol_id() final;

#pragma endregion

#pragma region node

		void debug(std::basic_ostream<char>& stream, int indent) const final;

#pragma endregion
	};
}
