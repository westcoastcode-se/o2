//
// Part of the o2 Project, under the Apache License v2.0 with o2 Project Exceptions.
// See the LICENSE file in the project root for license terms
//

#pragma once

#include "node_type.h"
#include "node_type_primitive.h"

namespace o2
{
	/**
	 * \brief a type in which we know the underlying type in compile time
	 *
	 * this is normally for primitive types or types with a known size
	 */
	class node_type_known_ref final
			: public node_type
	{
	public:
		node_type_known_ref(const source_code_view& view, node_type_primitive* primitive);

#pragma region node_type

		node_type* get_type() final
		{
			return _type;
		}

		compatibility is_compatible_with(node_type* rhs) const final
		{
			return _type->is_compatible_with(rhs);
		}

#pragma endregion

#pragma region node_symbol

		[[nodiscard]] string get_id() const final
		{
			return _type->get_id();
		}

#pragma endregion

#pragma region node

		void debug(debug_ostream& stream, int indent) const final;

#pragma endregion

	private:
		node_type* const _type;
	};
}