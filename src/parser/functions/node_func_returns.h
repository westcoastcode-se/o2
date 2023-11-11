//
// Part of the o2 Project, under the Apache License v2.0 with o2 Project Exceptions.
// See the LICENSE file in the project root for license terms
//

#pragma once

#include "../node.h"

namespace o2
{
	class node_func_returns
			: public node
	{
	public:
		explicit node_func_returns(const source_code_view& view);

#pragma region node

		void debug(std::basic_ostream<char>& stream, int indent) const final;

#pragma endregion
	};
}