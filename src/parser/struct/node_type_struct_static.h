//
// Part of the o2 Project, under the Apache License v2.0 with o2 Project Exceptions.
// See the LICENSE file in the project root for license terms
//

#pragma once

#include "../node.h"

namespace o2
{
	class node_type_struct_fields;

	/**
	 * \brief static container for structs
	 */
	class node_type_struct_static
			: public node
	{
	public:
		explicit node_type_struct_static(const source_code_view& view)
				: node(view), _fields()
		{
			set_query_access_flags(query_access_modifier_passthrough);
		}

		/**
		 * \return fields part of this struct
		 */
		node_type_struct_fields* get_fields() const
		{
			return _fields;
		}

#pragma region node

		void debug(std::basic_ostream<char>& stream, int indent) const final;

		node* on_child_added(node* n) final;

		void on_child_removed(node* n) final;

#pragma endregion

	private:
		node_type_struct_fields* _fields;
	};
}