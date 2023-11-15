//
// Part of the o2 Project, under the Apache License v2.0 with o2 Project Exceptions.
// See the LICENSE file in the project root for license terms
//

#pragma once

#include "../../node.h"

namespace o2
{
	/**
	 * \brief container for all functions inside the static block
	 */
	class node_type_struct_static_funcs
			: public node
	{
	public:
		explicit node_type_struct_static_funcs(const source_code_view& view)
				: node(view)
		{
			set_query_access_flags(query_access_modifier_passthrough);
		}

#pragma region node

		void debug(debug_ostream& stream, int indent) const final;

#pragma endregion
	};
}