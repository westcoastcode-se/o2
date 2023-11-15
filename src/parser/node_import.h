//
// Part of the o2 Project, under the Apache License v2.0 with o2 Project Exceptions.
// See the LICENSE file in the project root for license terms
//

#pragma once

#include "node.h"
#include "node_ref.h"
#include "node_package.h"

namespace o2
{
	/**
	 * \brief an import with a reference to other packages
	 */
	class node_import
			: public node
	{
	public:
		node_import(const source_code_view& view, string_view import_statement)
				: node_import(view, import_statement, string_view())
		{
		}

		node_import(const source_code_view& view, string_view import_statement, string_view alias)
				: node(view), _import_statement(import_statement), _alias(alias), _package()
		{
			set_query_access_flags(query_access_modifier_passthrough);
		}

		string_view get_import_statement() const
		{
			return _import_statement;
		}

		string_view get_alias() const
		{
			return _alias;
		}

		/**
		 * \return the package this import is found
		 */
		node_package* get_package() const
		{
			return _package;
		}

#pragma region node

		void debug(debug_ostream& stream, int indent) const final;

		bool resolve(const recursion_detector* rd) final;

		void query(query_node_visitor* visitor, int flags) final;

#pragma endregion

	private:
		const string_view _import_statement;
		node_package* _package;
		const string_view _alias;
	};
}
