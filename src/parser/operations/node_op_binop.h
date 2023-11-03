//
// Part of the o2 Project, under the Apache License v2.0 with o2 Project Exceptions.
// See the LICENSE file in the project root for license terms
//

#pragma once

#include "node_op.h"

namespace o2
{
	/**
	* \brief binary operator, with the syntax of <left> <op> <right>
	**/
	class node_op_binop
			: public node_op
	{
	public:
		enum op
		{
			minus,
			plus,
			mult,
			div,
			equals,
			not_equals,
			less_then,
			less_then_equals,
			greater_then,
			greater_then_equals,
			bit_and,
			bit_or,
			bit_xor,
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
			case token_type::minus:
				return minus;
			case token_type::plus:
				return plus;
			case token_type::pointer:
				return mult;
			case token_type::div:
				return div;
			case token_type::test_equals:
				return equals;
			case token_type::test_not_equals:
				return not_equals;
			case token_type::test_lt:
				return less_then;
			case token_type::test_lte:
				return less_then_equals;
			case token_type::test_gt:
				return greater_then;
			case token_type::test_gte:
				return greater_then_equals;
			case token_type::ref:
				return bit_and;
			case token_type::bit_or:
				return bit_or;
			case token_type::bit_xor:
				return bit_xor;
			default:
				return unknown;
			}
		}

		node_op_binop(const source_code_view& view, op o)
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
		 * \return the node on the left of this binary operator
		 */
		node* get_left() const;

		/**
		 * \return the node on the right of this binary operator
		 */
		node* get_right() const;

#pragma region node

		void debug(std::basic_ostream<char>& stream, int indent) const final;

#pragma endregion

	private:
		const op _operator;
	};

	/**
	 * \brief optimizer which merges two constants together
	 */
	class node_optimizer_binop_merge final
			: public node_op_optimizer<node_op_binop>
	{
	public:
		int count = 0;

		vector<node*> optimize(node* n) final;
	};
}