//
// Part of the o2 Project, under the Apache License v2.0 with o2 Project Exceptions.
// See the LICENSE file in the project root for license terms
//

#pragma once

#include "node_op.h"

namespace o2
{
	/**
	 * \brief return statement that returns the children as values
	 */
	class node_op_return
			: public node_op
	{
	public:
		explicit node_op_return(const source_code_view& view);

#pragma region node

		void debug(std::basic_ostream<char>& stream, int indent) const final;

#pragma endregion
	};
}