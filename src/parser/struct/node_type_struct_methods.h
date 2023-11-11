//
// Part of the o2 Project, under the Apache License v2.0 with o2 Project Exceptions.
// See the LICENSE file in the project root for license terms
//

#pragma once

#include "../node.h"

namespace o2
{
	class  node_type_struct_methods
			: public node
	{
	public:
		explicit node_type_struct_methods(const source_code_view& view)
				: node(view)
		{
			set_query_access_flags(query_access_modifier_passthrough);
		}

#pragma region node

		void debug(std::basic_ostream<char>& stream, int indent) const final;

#pragma endregion

	};
}
