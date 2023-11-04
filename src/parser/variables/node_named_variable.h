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
	 * \brief a variable with a scope-unique name
	 */
	class node_named_variable
			: public node_symbol
	{
	public:
		/**
		 * \brief modifiers that can be applied to a variable
		 */
		enum modifiers
		{
			modifier_const = 1 << 0
		};

		node_named_variable(const source_code_view& view, string_view name, int modifiers);

		/**
		 * \return the name of the variable
		 */
		string_view get_name() const
		{
			return _name;
		}

		/**
		 * \return the type
		 */
		node_type* get_type() const
		{
			return _type;
		}

#pragma region node

		void debug(std::basic_ostream<char>& stream, int indent) const final;

		node* on_child_added(node* n) final;

		void on_child_removed(node* n) final;

#pragma endregion

	private:
		const string_view _name;
		node_type* _type;
		const int _modifiers;
	};
}
