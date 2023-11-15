//
// Part of the o2 Project, under the Apache License v2.0 with o2 Project Exceptions.
// See the LICENSE file in the project root for license terms
//

#pragma once

#include "node_op.h"

namespace o2
{
	/**
	* \brief unary operator, with the syntax of <left> <op>
	**/
	class node_op_unaryop
			: public node_op
	{
	public:
		enum op
		{
			minus,
			plus,
			inc,
			dec,
			bit_not,
			not_,
			unknown
		};

		/**
		 * \brief convert the supplied token type into an unary operation
		 * \param t 
		 * \return 
		 */
		static op from_token_type(const token_type t)
		{
			switch (t)
			{
			case token_type::bit_not:
				return bit_not;
			case token_type::plus:
				return plus;
			case token_type::minus:
				return minus;
			case token_type::inc:
				return inc;
			case token_type::dec:
				return dec;
			case token_type::not_:
				return not_;
			default:
				return unknown;
			}
		}

		node_op_unaryop(const source_code_view& view, op o)
				: node_op(view), _operator(o)
		{
		}

		/**
		 * \return the operator
		 */
		op get_operator() const
		{
			return _operator;
		}

		/**
		 * \return the node on the right of this binary operator
		 */
		node* get_right() const;

#pragma region node

		void on_parent_node(node* n) final;

		void debug(debug_ostream& stream, int indent) const final;

#pragma endregion

	private:
		const op _operator;
	};

	/**
	 * \brief optimizer which merges a constants with the node's operator
	 */
	class node_optimizer_unaryop_merge final
			: public node_op_optimizer<node_op_unaryop>
	{
	public:
		int count = 0;

		vector<node*> optimize(node* n) final;
	};
}