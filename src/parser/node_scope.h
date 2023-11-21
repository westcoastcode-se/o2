//
// Part of the o2 Project, under the Apache License v2.0 with o2 Project Exceptions.
// See the LICENSE file in the project root for license terms
//

#pragma once

#include "node_symbol.h"

namespace o2
{
	class node_scope
			: public node_symbol
	{
	public:
		explicit node_scope(const source_code_view& view)
				: node_symbol(view)
		{
		}

#pragma region node_symbol

		[[nodiscard]] string get_id() const final;

#pragma endregion

#pragma region node

		void debug(debug_ostream& stream, int indent) const final;

#pragma endregion
	};
}