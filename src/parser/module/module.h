//
// Part of the o2 Project, under the Apache License v2.0 with o2 Project Exceptions.
// See the LICENSE file in the project root for license terms
//

#pragma once

#include "module_package_lookup.h"
#include "../source_code.h"
#include "node_module.h"

namespace o2
{
	class syntax_tree;

	class node_package;

	class node_import;

	/**
	 * \brief a module defines how a set of source code files relates to each other
	 */
	class module final
			: public memory_tracked
	{
	public:
		enum modifiers
		{
			// tells us that the module is added to the syntax tree
			modifier_added = 1 << 0
		};

		/**
		 * \param name the name of the module
		 * \param root_path path to where the the source code is found for this module
		 */
		module(string_view name, const std::filesystem::path& root_path)
				: _parent(), _name(name), _root_path(root_path),
				  _sources(new filesystem_module_package_lookup(root_path)),
				  _node_module(o2_new node_module(this)),
				  _modifiers()
		{
		}

		~module();

		/**
		 * \return true if this module is the root module
		 */
		[[nodiscard]] bool is_root_module() const
		{
			return _parent == nullptr;
		}

		/**
		 * \return the parent module
		 */
		[[nodiscard]] module* get_parent() const
		{
			return _parent;
		}

		/**
		 * \return the name of the module
		 */
		[[nodiscard]] string_view get_name() const
		{
			return _name;
		}

		/**
		 * \return the full path to where the module root is found
		 */
		[[nodiscard]] const std::filesystem::path& get_root_path() const
		{
			return _root_path;
		}

		/**
		 * \return all requirements
		 */
		[[nodiscard]] array_view<module*> get_requirements() const
		{
			return _requirements;
		}

		/**
		 * \brief get the relative path from this module's point of view. it's assumed that
		 *        you've verified that the path is compatible with this module beforehand
		 * \param import_path the full import path
		 * \return a relative path from this module's point of view
		 */
		[[nodiscard]] string_view get_relative_path(string_view import_path) const;

		// represents no match
		static const int MATCHES_NO_MATCH = INT32_MAX;

		/**
		 * \param import_path a full import statement
		 * \return a value on how close the import path is to the module. 0 is a perfect match
		 * 		   and MATCHES_NO_MATCH is no match at all
		 */
		[[nodiscard]] int matches(string_view import_path) const;

		/**
		 * \brief get the package information for the supplied import path
		 * \param import_path the path supplied in the imports statement
		 * \return information on the package's source code
		 *
		 * it is assumed that you've checked compatibility with the matches method before calling this
		 */
		[[nodiscard]] package_source_info* get_package_info(string_view import_path) const;

		/**
		 * \brief load the supplied source code content
		 * \param info where to put the loaded source code into
		 */
		void load_package_sources(package_source_info* info) const;

		/**
		 * \brief add this module to the syntax tree
		 * \param st a syntax tree
		 * \return the node we added to the syntax tree
		 */
		node_module* insert_into(syntax_tree* st);

		/**
		 * \brief add the supplied package to this module
		 * \param p the package
		 * \return the added package
		 */
		node_package* add_package(node_package* p);

		/**
		 * \brief package is now imported
		 * \param p
		 *
		 * when a package is marked as imported then all imports that's waiting for that package
		 * will be marked as imported. This will, potentially, start the resolve phase of the package
		 * where the import is found
		 */
		void notify_package_imported(node_package* p);

		/**
		 * \brief add an import that's being loaded by this module
		 * \param i the import that's being loaded
		 *
		 * this is a way for a module to know which import is dependent on it
		 */
		void add_import_request(node_import* i);

	private:
		module* const _parent;
		const string _name;
		const std::filesystem::path _root_path;
		module_package_lookup* const _sources;
		vector<module*> _requirements;
		node_module* const _node_module;
		int _modifiers;

		struct state
		{
			struct _parse
			{
				// imports that's waiting to be loaded
				vector<node_import*> import_requests;
			} parse;
		} _state;
	};
}
