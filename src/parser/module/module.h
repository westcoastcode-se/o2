//
// Part of the o2 Project, under the Apache License v2.0 with o2 Project Exceptions.
// See the LICENSE file in the project root for license terms
//

#pragma once

#include <utility>

#include "module_package_lookup.h"
#include "../source_code.h"
#include "node_module.h"

namespace o2
{
	class syntax_tree;

	class node_package;

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
		 * \param root_path path to where the the source code is found for this moduke
		 */
		module(string_view name, const std::filesystem::path& root_path)
				: _parent(), _name(name), _root_path(root_path),
				  _sources(new filesystem_module_package_lookup(root_path)),
				  _node_module(o2_new node_module(this)),
				  _modifiers()
		{
		}

		~module() final;

		/**
		 * \return true if this module is the root module
		 */
		bool is_root_module() const
		{
			return _parent == nullptr;
		}

		/**
		 * \return the parent module
		 */
		module* get_parent() const
		{
			return _parent;
		}

		/**
		 * \return the name of the module
		 */
		string_view get_name() const
		{
			return _name;
		}

		/**
		 * \return the full path to where the module root is found
		 */
		const std::filesystem::path& get_root_path() const
		{
			return _root_path;
		}

		/**
		 * \return all requirements
		 */
		array_view<module*> get_requirements() const
		{
			return _requirements;
		}

		/**
		 * \brief get the relative path from this module's point of view. it's assumed that
		 *        you've verified that the path is compatible with this module beforehand
		 * \param import_path the full import path
		 * \return a relative path from this module's point of view
		 */
		string_view get_relative_path(string_view import_path) const;

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
		 * \param st
		 * \return
		 */
		node_module* insert_into(syntax_tree* st);

		/**
		 * \brief add the supplied package to this module
		 * \param p
		 * \return
		 */
		node_package* add_package(node_package* p);

	private:
		module* const _parent;
		const string _name;
		const std::filesystem::path _root_path;
		module_package_lookup* const _sources;
		vector<module*> _requirements;
		node_module* const _node_module;
		int _modifiers;
	};
}
