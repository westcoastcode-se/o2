//
// Part of the o2 Project, under the Apache License v2.0 with o2 Project Exceptions.
// See the LICENSE file in the project root for license terms
//

#pragma once

#include "../node_type.h"
#include "node_type_struct_field.h"
#include "node_type_struct_methods.h"
#include "node_type_struct_inherits.h"
#include "../static/node_type_static_scope.h"
#include "../../node_attribute.h"

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
		[[nodiscard]] string_view get_name() const
		{
			return _name;
		}

		/**
		 * \return all attributes attached to this struct
		 */
		[[nodiscard]] node_attributes* get_attributes() const
		{
			return _attributes;
		}

		/**
		 * \return inheritances
		 */
		[[nodiscard]] node_type_struct_inherits* get_inherits() const
		{
			return _inherits;
		}

		/**
		 * \return fields part of this struct
		 */
		[[nodiscard]] node_type_struct_fields* get_fields() const
		{
			return _fields;
		}

		/**
		 * \return all fields part of this struct - included from all inherited structures
		 */
		[[nodiscard]] vector<node_type_struct_field*> get_all_fields() const;

		/**
		 * \return methods container
		 */
		[[nodiscard]] node_type_struct_methods* get_methods() const
		{
			return _methods;
		}

		/**
		 * \return static container
		 */
		[[nodiscard]] node_type_static_scope* get_static() const
		{
			return _static;
		}

		/**
		 * \brief search for an inherited type that matches the supplied symbol id
		 * \tparam str
		 * \param name
		 * \param s
		 * \return
		 */
		[[nodiscard]] node_type* find_inherited_type(string_view id) const
		{
			if (_inherits == nullptr)
				return nullptr;
			return _inherits->find_inherited_type(id);
		}

		/**
		 * \param type
		 * \return true if this struct inherits from the supplied type
		 */
		[[nodiscard]] bool inherits_from_type(node_type* type) const
		{
			if (_inherits == nullptr)
				return false;
			return _inherits->inherits_from_type(type);
		}

#pragma region node_type

		compatibility is_compatible_with(node_type* rhs) const override;

#pragma endregion

#pragma region node_symbol

		[[nodiscard]] string get_id() const final;

		bool superficial_test_symbol_collision(const node_type_struct* rhs) const;

#pragma endregion

#pragma region node

		void debug(debug_ostream& stream, int indent) const final;

		node* on_child_added(node* n) final;

		void on_child_removed(node* n) final;

		void on_parent_node(node* p) final;

		void write_json_properties(json& j) final;

		void on_process_phase(const recursion_detector* rd, resolve_state* state, int phase) final;

#pragma endregion

#pragma region json_serializable

		[[nodiscard]] string_view get_json_type() const override
		{
			return { "type_struct" };
		}

#pragma endregion

	private:
		string_view _name;
		node_attributes* _attributes;
		node_type_struct_inherits* _inherits;
		node_type_struct_fields* _fields;
		node_type_struct_methods* _methods;
		node_type_static_scope* _static;
	};
}