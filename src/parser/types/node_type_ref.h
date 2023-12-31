//
// Part of the o2 Project, under the Apache License v2.0 with o2 Project Exceptions.
// See the LICENSE file in the project root for license terms
//

#pragma once

#include "node_type.h"
#include "../node_ref.h"
#include "node_type_primitive.h"

namespace o2
{
	/**
	 * \brief node that has reference children that, eventually, resolve into a type of some kind
	 */
	class node_type_ref
			: public node_type
	{
	public:
		explicit node_type_ref(const source_code_view& view);

#pragma region node_type

		node_type* get_type() final
		{
			return _type;
		}

		compatibility is_compatible_with(node_type* rhs) const final;

#pragma endregion

#pragma region node_symbol

		[[nodiscard]] string get_id() const final;

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
