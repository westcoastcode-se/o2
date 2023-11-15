//
// Part of the o2 Project, under the Apache License v2.0 with o2 Project Exceptions.
// See the LICENSE file in the project root for license terms
//

#pragma once

#include "node_type.h"

namespace o2
{
	/**
	 * \brief a pointer of a type
	 */
	class node_type_accessor
			: public node_type
	{
	public:
		enum accessor_type
		{
			accessor_pointer,
			accessor_reference
		};

		node_type_accessor(const source_code_view& view, accessor_type t);

		accessor_type get_accessor() const
		{
			return _accessor;
		}

#pragma region node_type

		node_type* get_type()
		{
			return this;
		}

		int resolve_size(const recursion_detector* rd) final
		{
			return get_size();
		}

#pragma endregion

#pragma region node

		void debug(debug_ostream& stream, int indent) const final;

		bool resolve(const recursion_detector* rd) final;

#pragma endregion

	private:
		const accessor_type _accessor;
	};
}