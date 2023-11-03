//
// Part of the o2 Project, under the Apache License v2.0 with o2 Project Exceptions.
// See the LICENSE file in the project root for license terms
//

#pragma once

#include "node_type.h"
#include "../node_ref.h"

namespace o2
{
	/**
	 * \brief node that has reference children that, eventually, resolve into a type of some kind
	 */
	class node_type_ref
			: public node_type
	{
	public:
		node_type_ref(const source_code_view& view);

		/**
		 * \brief 
		 * \param ref the underlying reference of the type we are looking for
		 */
		void set_ref(node_ref* ref);

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
