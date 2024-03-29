//
// Part of the o2 Project, under the Apache License v2.0 with o2 Project Exceptions.
// See the LICENSE file in the project root for license terms
//

#pragma once

#include "node_type_static_scope_funcs.h"
#include "node_type_static_scope_vars.h"

namespace o2
{
	class node_type_complex_fields;

	/**
	 * \brief static container for types such as structs, classes and interfaces
	 */
	class node_type_static_scope
			: public node
	{
	public:
		explicit node_type_static_scope(const source_code_view& view)
				: node(view), _vars(), _funcs()
		{
			set_query_access_flags(query_access_modifier_passthrough);
		}

		/**
		 * \return all variables part of this static block
		 */
		[[nodiscard]] node_type_static_scope_vars* get_vars() const
		{
			return _vars;
		}

		/**
		 * \return functions part of this static block
		 */
		[[nodiscard]] node_type_static_scope_funcs* get_funcs() const
		{
			return _funcs;
		}

#pragma region node

		void debug(debug_ostream& stream, int indent) const final;

		node* on_child_added(node* n) final;

		void on_child_removed(node* n) final;

#pragma endregion

	private:
		node_type_static_scope_vars* _vars;
		node_type_static_scope_funcs* _funcs;
	};
}