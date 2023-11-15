//
// Part of the o2 Project, under the Apache License v2.0 with o2 Project Exceptions.
// See the LICENSE file in the project root for license terms
//

#pragma once

#include "node_op.h"
#include "../variables/node_var.h"

namespace o2
{
	class node_type_implicit;

	/**
	 * \brief a node that represents the assignment operator
	 *
	 * This node requires two things, first a reference to the variable we are assigning
	 * and secondly the expression we are assigning.
	 *
	 * The reference can be a node_ref or a node_link
	 */
	class node_op_assign
			: public node_op
	{
	public:
		explicit node_op_assign(const source_code_view& view);

		/**
		 * \return the variable we are setting
		 */
		node_var* get_variable() const
		{
			assert(_variable != nullptr && "you must resolve this node's references");
			return _variable;
		}

		/**
		 * \return eventually, the value we want the variable to become
		 */
		node_op* get_expression() const
		{
			assert(_variable != nullptr && "assignment does not have an expression");
			return _expression;
		}

#pragma region node_op

		node_type* get_type() final;

#pragma endregion

#pragma region node

		node* on_child_added(node* n) final;

		void on_child_removed(node* n) final;

		void debug(debug_ostream& stream, int indent) const final;

#pragma endregion

	private:
		node_var* _variable;
		node_op* _expression;
	};
}