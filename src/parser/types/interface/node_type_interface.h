//
// Part of the o2 Project, under the Apache License v2.0 with o2 Project Exceptions.
// See the LICENSE file in the project root for license terms
//

#pragma once

#include "../node_type.h"
#include "../static/node_type_static_scope.h"
#include "../../node_attribute.h"

namespace o2
{
	/**
	 * \brief a type that represents an interface
	 *
	 * interfaces are only allowed to have:
	 * - static variables
	 * - static functions
	 * - methods without bodies
	 */
	class node_type_interface final
			: public node_type
	{
	public:
		node_type_interface(const source_code_view& view, string_view name);

	private:
		const string_view _name;
		node_type_static_scope* _static;
	};
}