//
// Part of the o2 Project, under the Apache License v2.0 with o2 Project Exceptions.
// See the LICENSE file in the project root for license terms
//

#pragma once

#include "node_symbol.h"
#include "variables/node_var.h"
#include <vector>
#include <unordered_map>

namespace o2
{
	/**
	 * \brief package containing packages, functions, constants etc.
	 */
	class node_package
			: public node_symbol
	{
	public:
		node_package(const source_code_view& view, string_view name)
				: node_symbol(view), _name(name)
		{
		}

		/**
		 * \return the name of the package
		 */
		inline string_view get_name() const
		{
			return _name;
		}

#pragma region node_symbol

		void resolve_symbol_id() final;

#pragma endregion

#pragma region node

		node* on_child_added(node* n) final;

		void on_child_removed(node* n) final;

		void debug(debug_ostream& stream, int indent) const final;

		void query(query_node_visitor* visitor, int flags) final;

#pragma endregion

	private:
		const string_view _name;
		std::unordered_map<string_view, node_var*> _variables;
	};

}