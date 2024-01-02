//
// Part of the o2 Project, under the Apache License v2.0 with o2 Project Exceptions.
// See the LICENSE file in the project root for license terms
//

#pragma once

#include "../node.h"
#include "../types/node_type.h"

namespace o2
{
	class node_type_struct_fields
			: public node
	{
	public:
		explicit node_type_struct_fields(const source_code_view& view);

#pragma region node

		void debug(debug_ostream& stream, int indent) const final;

#pragma endregion

	};

	/**
	 * \brief a field inside a struct
	 */
	class node_type_struct_field
			: public node_symbol
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

		/**
		 * \return the size, in bytes, that this field takes in memory
		 */
		[[nodiscard]] int get_size() const
		{
			return _size;
		}

#pragma region node_symbol

		[[nodiscard]] string get_id() const final;

		bool superficial_test_symbol_collision(const node_type_struct_field* rhs) const;

#pragma endregion

#pragma region node

		void debug(debug_ostream& stream, int indent) const final;

		void resolve0(const recursion_detector* rd, resolve_state* state) final;

		void on_process_phase(const recursion_detector* rd, resolve_state* state, int phase) final;

		node* on_child_added(node* n) final;

		void on_child_removed(node* n) final;

		void on_parent_node(node* p) final;

#pragma endregion

	private:
		const string_view _name;
		int _padding;
		int _size;
		node_type* _field_type;
	};
}
