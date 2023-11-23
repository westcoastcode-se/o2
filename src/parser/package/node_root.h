//
// Part of the o2 Project, under the Apache License v2.0 with o2 Project Exceptions.
// See the LICENSE file in the project root for license terms
//

#pragma once

#include "../node_symbol.h"

namespace o2
{
	/**
	 * \brief represents the root node for the entire syntax tree
	 */
	class node_root final
			: public node_symbol
	{
	public:
		node_root()
				: node_symbol(source_code_view())
		{
		}

#pragma region node_symbol

		[[nodiscard]] string get_id() const final
		{
			return { "/" };
		}

#pragma endregion

#pragma region node

		void write_json_properties(json& j) final;

#pragma endregion

#pragma region json_serializable

		[[nodiscard]] string_view get_json_type() const override
		{
			return { "root" };
		}

#pragma endregion
	};
}