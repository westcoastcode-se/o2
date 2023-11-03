//
// Part of the o2 Project, under the Apache License v2.0 with o2 Project Exceptions.
// See the LICENSE file in the project root for license terms
//

#pragma once

#include "../node.h"

namespace o2
{
	template<class T>
	class node_op_optimizer
		: public node_optimizer
	{
	public:
		bool accept(const node* n) override { return dynamic_cast<const T*>(n) != nullptr; }
	};

	/**
	 * \brief base class for operations
	 */
	class node_op
		: public node
	{
	public:
		node_op(const source_code_view& view)
			: node(view) {}
	};
}