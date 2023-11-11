//
// Part of the o2 Project, under the Apache License v2.0 with o2 Project Exceptions.
// See the LICENSE file in the project root for license terms
//

#pragma once

#include "node_type.h"
#include "node_type_struct_field.h"
#include "node_type_struct_methods.h"

namespace o2
{
	/**
	 * \brief a struct
	 */
	class node_type_struct
			: public node_type
	{
	public:
		node_type_struct(const source_code_view& view, string_view name);

		/**
		 * \return the name of the struct
		 */
		string_view get_name() const
		{
			return _name;
		}

		/**
		 * \return fields part of this struct
		 */
		node_type_struct_fields* get_fields() const
		{
			return _fields;
		}

		/**
		 * \return all fields part of this struct - included from all inherited structures
		 */
		vector<node_type_struct_field*> get_all_fields() const;

		/**
		 * \return methods container
		 */
		node_type_struct_methods* get_methods() const
		{
			return _methods;
		}

#pragma region node_symbol

		void resolve_symbol_id() final;

#pragma endregion

#pragma region node_type

		node_type* get_type()
		{
			return this;
		}

		int resolve_size(const recursion_detector* rd) final;

#pragma endregion

#pragma region node

		void debug(std::basic_ostream<char>& stream, int indent) const final;

		node* on_child_added(node* n) final;

		void on_child_removed(node* n) final;

		void on_parent_node(node* p) final;

#pragma endregion

	private:
		string_view _name;
		node_type_struct_fields* _fields;
		node_type_struct_methods* _methods;
	};
}