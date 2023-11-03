//
// Part of the o2 Project, under the Apache License v2.0 with o2 Project Exceptions.
// See the LICENSE file in the project root for license terms
//

#pragma once

#include "node.h"

namespace o2
{
	/**
	 * \brief an attribute that can be attached to types, functions, methods, arguments and fields in a type
	 */
	class node_attribute
			: public node
	{
	public:
		node_attribute(const source_code_view& view);
	};
}