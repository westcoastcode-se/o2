//
// Part of the o2 Project, under the Apache License v2.0 with o2 Project Exceptions.
// See the LICENSE file in the project root for license terms
//

#pragma once

#include "../node_symbol.h"
#include "../types/node_type_ref.h"

namespace o2
{
	/**
	 * \brief base class for variables
	 */
	class node_var
			: public node_symbol
	{
	public:
		enum modifier
		{
			modifier_const = 1 << 0,
			modifier_readonly = 1 << 1
		};

		node_var(const source_code_view& view, string_view name, int modifiers);

		/**
		 * \return the name of the variable
		 */
		string_view get_name() const
		{
			return _name;
		}

		/**
		 * \return modifiers applied to this variable
		 */
		[[nodiscard]] int get_modifiers() const
		{
			return _modifiers;
		}

		/**
		 * \return true if this variable a const variable
		 */
		[[nodiscard]] bool is_const() const
		{
			return bit_isset(_modifiers, modifier_const);
		}

		/**
		 * \return the type
		 */
		[[nodiscard]] node_type* get_type() const
		{
			return _type;
		}

#pragma region node_symbol

		[[nodiscard]] string get_id() const final;

#pragma endregion

#pragma region node

		void debug(debug_ostream& stream, int indent) const override;

		void write_json_properties(json& j) override;

		node* on_child_added(node* n) override;

		void on_child_removed(node* n) override;

		void on_parent_node(node* p) final;

#pragma endregion

	protected:
		const string_view _name;
		const int _modifiers;
		node_type* _type;
	};
}
