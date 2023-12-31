//
// Part of the o2 Project, under the Apache License v2.0 with o2 Project Exceptions.
// See the LICENSE file in the project root for license terms
//

#pragma once

#include "node_func.h"

namespace o2
{
	class node_var_this;

	/**
	 * \brief a method function
	 */
	class node_func_method
			: public node_func
	{
	public:
		node_func_method(const source_code_view& view, string_view name)
				: node_func(view, name), _this()
		{
		}

		node_var_this* get_this() const
		{
			return _this;
		}


#pragma region node_symbol

		[[nodiscard]] string get_id() const override;

#pragma endregion

#pragma region node

		void debug(debug_ostream& stream, int indent) const final;

#pragma endregion

	private:
		node_var_this* _this;
	};
}
