//
// Part of the o2 Project, under the Apache License v2.0 with o2 Project Exceptions.
// See the LICENSE file in the project root for license terms
//

#pragma once

#include "node_type.h"

namespace o2
{
	/**
	 * \brief a reference of a specific type
	 */
	class node_type_reference_of final
			: public node_type
	{
	public:
		explicit node_type_reference_of(const source_code_view& view)
				: node_type(view, sizeof(void*)), _reference_of()
		{
		}

		/**
		 * \return the type this pointer is of (for example: "*byte" returns the type of "byte")
		 */
		[[nodiscard]] node_type* get_reference_of_type() const
		{
			return _reference_of;
		}

#pragma region node_type

		int resolve_size(const recursion_detector* rd) final
		{
			return get_size();
		}

		compatibility is_compatible_with(node_type* rhs) const final;

#pragma endregion

#pragma region node_symbol

		[[nodiscard]] string get_id() const override;

#pragma endregion

#pragma region node

		void debug(debug_ostream& stream, int indent) const final;

		node* on_child_added(node* n) final;

		void on_child_removed(node* n) final;

#pragma endregion

	private:
		node_type* _reference_of;
	};
}