//
// Part of the o2 Project, under the Apache License v2.0 with o2 Project Exceptions.
// See the LICENSE file in the project root for license terms
//

#pragma once

#include "../memory.h"
#include "../source_code.h"
#include "../source_code_view.h"
#include "../collections/vector.h"

namespace o2
{
	class module_source_codes;

	/**
	 * \brief a module defines how a set of source code files relates to each other
	 */
	class module
			: public memory_tracked
	{
	public:
		/**
		 * \param name the name of the module
		 * \param root_path path to where the root can be found for this module
		 * \param lookup type used when fetching source code managed by this module
		 */
		module(string_view name, string_view root_path,
				module_source_codes* lookup)
				: _parent(), _name(name), _root_path(root_path), _sources(lookup)
		{
		}

		/**
		 * \param name the name of the module
		 * \param root_path path to where the root can be found for this module
		 * \param lookup type used when fetching source code managed by this module
		 */
		module(string_view name, string_view root_path,
				module_source_codes* lookup,
				std::initializer_list<module*> requirements)
				: _parent(), _name(name), _root_path(root_path), _sources(lookup),
				  _requirements(requirements)
		{
		}

		module(string_view name, string_view root_path,
				module_source_codes* lookup,
				vector<module*> requirements)
				: _parent(), _name(name), _root_path(root_path), _sources(lookup),
				  _requirements(std::move(requirements))
		{
		}

		~module();

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
		string_view get_root_path() const
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
		 * \return the relative path
		 */
		string_view get_relative_path(string_view path) const;

		/**
		 * \param import_path
		 * \return a value on how close the import path is to the module. 0 is a perfect match
		 */
		int matches(string_view import_path) const;

		/**
		 * \brief get all files found at the supplied relative path
		 * \param import_path the path supplied in the imports statement
		 * \return a list of all source codes for this path
		 *
		 * it is assumed that you've checked compatibility with the matches method before calling this
		 */
		array_view<source_code*> imported_files(string_view import_path, bool* loaded) const;

	private:
		module* const _parent;
		const string_view _name;
		const string_view _root_path;
		module_source_codes* const _sources;
		vector<module*> _requirements;
	};
}
