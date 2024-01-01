//
// Part of the o2 Project, under the Apache License v2.0 with o2 Project Exceptions.
// See the LICENSE file in the project root for license terms
//

#pragma once

#include "node_type.h"

namespace o2
{
	/**
	 * \brief a type that is resolved by a child node_op
	 *
	 * the implicit type is used in the following use-cases:
	 * - const <name> = <expr>
	 * - var <name> = <expr>
	 * - var <name1>,<name2> = <expr>, <expr>
	 * - <name> := <expr>
	 * - <name1>,<name2> := <expr>, <expr>
	 * - func <name>() -> <expr>
	 * - const func <name>() -> <expr>
	 *
	 * implicit types must have a child node that associates itself with
	 * an operation node that has a real type.
	 *
	 *
	 */
	class node_type_implicit
			: public node_type
	{
	public:
		explicit node_type_implicit(const source_code_view& view);

#pragma region node_symbol

		[[nodiscard]] string get_id() const final;

#pragma endregion

#pragma region node_type

		node_type* get_type() final
		{
			return _type;
		}

#pragma endregion

#pragma region node

		void debug(debug_ostream& stream, int indent) const final;

		void resolve0(const recursion_detector* rd, resolve_state* state) final;

		void on_process_phase(const recursion_detector* rd, resolve_state* state, int phase) final;

#pragma endregion

	private:
		node_type* _type;
	};
}