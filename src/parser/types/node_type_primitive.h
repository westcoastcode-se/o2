//
// Part of the o2 Project, under the Apache License v2.0 with o2 Project Exceptions.
// See the LICENSE file in the project root for license terms
//

#pragma once

#include "node_type.h"

namespace o2
{
	class node_type_primitive
			: public node_type
	{
	public:
		node_type_primitive(const vector<string_view>& names, int size);

		/**
		 * \return the name of the primitive
		 */
		string_view get_name() const
		{
			return _names[0];
		}

		/**
		 * \return all names that matches this primitive
		 */
		array_view<string_view> get_names() const
		{
			return _names;
		}

#pragma region node_type

		node_type* get_type()
		{
			return this;
		}

		int resolve_size(const recursion_detector*) final
		{
			return get_size();
		}

#pragma endregion

#pragma region node

		bool resolve(const recursion_detector* rd) final
		{
			return true;
		}

		void debug(debug_ostream& stream, int indent) const final;

#pragma endregion

	private:
		vector<string_view> _names;
	};

}