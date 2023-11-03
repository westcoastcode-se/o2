//
// Part of the o2 Project, under the Apache License v2.0 with o2 Project Exceptions.
// See the LICENSE file in the project root for license terms
//

#pragma once

#include "node_type.h"

namespace o2
{
	/**
	 * \brief a type that is resolved by it's children
	 */
	class node_type_implicit
			: public node_type
	{
	public:
		explicit node_type_implicit(const source_code_view& view);

#pragma region node_type

		node_type* get_type() final
		{
			return _type;
		}

		int resolve_size(const recursion_detector* rd) final;

#pragma endregion

#pragma region node

		void debug(std::basic_ostream<char>& stream, int indent) const final;

		bool resolve(const recursion_detector* rd) final;

#pragma endregion

	private:
		node_type* _type;
	};
}