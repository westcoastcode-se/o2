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
		// the loading status of the import
		enum status
		{
			// this import is not loaded yet
			not_loaded = 0,
			// this import is potentially loaded
			status_loaded = 1 << 0
		};

		node_import(const source_code_view& view, string_view import_statement)
				: node_import(view, import_statement, string_view())
		{
		}

		node_import(const source_code_view& view, string_view import_statement, string_view alias)
				: node(view), _import_statement(import_statement), _alias(alias), _package(), _status(not_loaded)
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

		/**
		 * \brief method called when this import is imported
		 * \return true if all imports are imported for this package
		 */
		bool notify_imported();

#pragma region node

		void on_parent_node(node* parent) final;

		void on_removed_parent_node(node* parent) final;

		void debug(debug_ostream& stream, int indent) const final;

		bool resolve(const recursion_detector* rd) final;

		void query(query_node_visitor* visitor, int flags) final;

#pragma endregion

	private:
		const string_view _import_statement;
		node_package* _package;
		const string_view _alias;
		status _status;
	};
}
