//
// Part of the o2 Project, under the Apache License v2.0 with o2 Project Exceptions.
// See the LICENSE file in the project root for license terms
//

#pragma once

#include "node_type.h"

namespace o2
{
	/**
	 * \brief an array of a type
	 *
	 * the number of elements in the array has to be compile time knowable
	 */
	class node_type_array
			: public node_type
	{
	public:
		node_type_array(const source_code_view& view);
		node_type_array(const source_code_view& view, int count);

		/**
		 * \return the number of elements in this array
		 */
		int get_array_count() const
		{
			return _count;
		}

#pragma region node_type

		node_type* get_type()
		{
			return this;
		}

		int resolve_size(const recursion_detector* rd) final;

#pragma endregion

#pragma region node

		void debug(debug_ostream& stream, int indent) const final;

		bool resolve(const recursion_detector* rd) final;

#pragma endregion

	private:
		int _count;
	};
}