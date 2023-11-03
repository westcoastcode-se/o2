//
// Part of the o2 Project, under the Apache License v2.0 with o2 Project Exceptions.
// See the LICENSE file in the project root for license terms
//

#pragma once

#include "node_op.h"
#include "../primitive_value.h"

namespace o2
{
	/**
	 * \brief A constant value
	 */
	class node_op_constant
			: public node_op
	{
	public:
		node_op_constant(const source_code_view& view, const primitive_value& value)
				: node_op(view, modifier_const), _value(value)
		{
		}

		/**
		 * \return the constant value
		 */
		const primitive_value& get_value() const
		{
			return _value;
		}

#pragma region node

		void debug(std::basic_ostream<char>& stream, int indent) const final;

#pragma endregion

	private:
		primitive_value _value;
	};
}