//
// Part of the o2 Project, under the Apache License v2.0 with o2 Project Exceptions.
// See the LICENSE file in the project root for license terms
//

#pragma once

#include "../node.h"
#include "node_type.h"

namespace o2
{
	class node_type_struct_fields
			: public node
	{
	public:
		node_type_struct_fields(const source_code_view& view);

#pragma region node

		void debug(std::basic_ostream<char>& stream, int indent) const final;

#pragma endregion

	};

	/**
	 * \brief a field inside a struct
	 */
	class node_type_struct_field
			: public node
	{
	public:
		node_type_struct_field(const source_code_view& view, string_view name);

		/**
		 * \return the name of the field
		 */
		string_view get_name() const
		{
			return _name;
		}

		/**
		 * \return extra bytes that this field takes
		 */
		int get_padding() const
		{
			return _padding;
		}

		/**
		 * \return the type this field is of
		 */
		node_type* get_field_type()
		{
			return _field_type;
		}

#pragma region node

		void debug(std::basic_ostream<char>& stream, int indent) const final;

		bool resolve(const recursion_detector* rd) final;

		node* on_child_added(node* n) final;

		void on_child_removed(node* n) final;

#pragma endregion

	private:
		const string_view _name;
		int _padding;
		node_type* _field_type;
	};
}
