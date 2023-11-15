//
// Part of the o2 Project, under the Apache License v2.0 with o2 Project Exceptions.
// See the LICENSE file in the project root for license terms
//

#pragma once

#include "node_op.h"

namespace o2
{
	class node_func;

	/**
	 * \brief call a function
	 */
	class node_op_callfunc
			: public node_op
	{
	public:
		explicit node_op_callfunc(const source_code_view& view)
				: node_op(view), _func()
		{
		}

		/**
		 * \return the function this operation will call
		 */
		node_func* get_func() const
		{
			return _func;
		}

#pragma region node_op

		node_type* get_type() final;

#pragma endregion

#pragma region node

		node* on_child_added(node* n) final;

		void on_child_removed(node* n) final;

		void debug(debug_ostream& stream, int indent) const final;

		bool resolve(const recursion_detector* rd) final;

#pragma endregion

	private:
		node_func* _func;
	};
}