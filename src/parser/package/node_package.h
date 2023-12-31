//
// Part of the o2 Project, under the Apache License v2.0 with o2 Project Exceptions.
// See the LICENSE file in the project root for license terms
//

#pragma once

#include "../node_symbol.h"
#include "../variables/node_var.h"
#include <vector>
#include <unordered_map>

namespace o2
{
	class node_import;

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
		[[nodiscard]] string_view get_name() const
		{
			return _name;
		}

		/**
		 * \brief method called when the supplied import is added as a child node for this package
		 * \param i
		 */
		void on_import_added(node_import* i);

		/**
		 * \brief method called when the supplied import is removed as a child
		 * \param i the import
		 * \return true if all imports are imported
		 */
		bool on_import_removed(node_import* i);

		/**
		 * \brief add a package to be resolved after this one
		 * \param p
		 */
		void on_resolved_before(node_package* p);

		/**
		 * \brief start processing all post-parse phases
		 */
		void process_phases();

#pragma region node_symbol

		[[nodiscard]] string get_id() const override;

		bool superficial_test_symbol_collision(const node_package* rhs) const;

#pragma endregion

#pragma region node

		void on_parent_node(node* p) final;

		node* on_child_added(node* n) final;

		void on_child_removed(node* n) final;

		void debug(debug_ostream& stream, int indent) const final;

		void query(query_node_visitor* visitor, int flags) final;

		void write_json_properties(json& j) override;

#pragma endregion

#pragma region json_serializable

		[[nodiscard]] string_view get_json_type() const override
		{
			return { "package" };
		}

#pragma endregion

	private:

		void process_phase_resolve(const recursion_detector* rd, resolve_state* state);

		void process_phase_resolve_size(const recursion_detector* rd, resolve_state* state);

	private:
		const string_view _name;
		std::unordered_map<string_view, node_var*> _variables;

		// a mutable state that's used during this packages parse, resolve and link phase
		struct state
		{
			// state used during the parse phase.
			struct _parse
			{
				// a collection of all imports that this package is waiting for to be
				// resolved
				vector<node_import*> pending_imports;
				// packages that's waiting for this package until it, itself, can be resolved
				vector<node_package*> depended_resolves;
			} parse;
		} _state;
	};

}