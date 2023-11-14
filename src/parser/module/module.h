//
// Part of the o2 Project, under the Apache License v2.0 with o2 Project Exceptions.
// See the LICENSE file in the project root for license terms
//

#pragma once

#include <utility>

#include "module_source_code.h"
#include "../node.h"
#include "../source_code.h"

namespace o2
{
	/**
	 * \brief a module defines how a set of source code files relates to each other
	 */
	class module final
			: public node
	{
	public:
		/**
		 * \param name the name of the module
		 * \param root_path path to where the root can be found for this module
		 * \param lookup type used when fetching source code managed by this module
		 */
		module(string_view name, string_view root_path,
				module_source_codes* lookup)
				: node(source_code_view()),
				  _parent(), _name(name), _root_path(root_path), _sources(lookup)
		{
		}

		/**
		 * \param name the name of the module
		 * \param root_path path to where the root can be found for this module
		 * \param lookup type used when fetching source code managed by this module
		 */
		module(string name, string root_path,
				module_source_codes* lookup)
				: node(source_code_view()),
				  _parent(), _name(std::move(name)), _root_path(std::move(root_path)), _sources(lookup)
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
				: node(source_code_view()),
				  _parent(), _name(name), _root_path(root_path), _sources(lookup),
				  _requirements(requirements)
		{
		}

		module(string_view name, string_view root_path,
				module_source_codes* lookup,
				vector<module*> requirements)
				: node(source_code_view()),
				  _parent(), _name(name), _root_path(root_path), _sources(lookup),
				  _requirements(std::move(requirements))
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
		 * \brief get the relative path from this module's point of view. it's assumed that
		 *        you've verified that the path is compatible with this module beforehand
		 * \param import_path the full import path
		 * \return a relative path from this module's point of view
		 */
		string_view get_relative_path(string_view import_path) const;

		/**
		 * \param import_path
		 * \return a value on how close the import path is to the module. 0 is a perfect match
		 */
		int matches(string_view import_path) const;

		/**
		 * \brief get all files found at the supplied relative path
		 * \param import_path the path supplied in the imports statement
		 * \return information on the package's source code
		 *
		 * it is assumed that you've checked compatibility with the matches method before calling this
		 */
		package_source_code* imported_files(string_view import_path) const;

#pragma region node

		void debug(std::basic_ostream<char>& stream, int indent) const final;

#pragma endregion

	private:
		module* const _parent;
		const string _name;
		const string _root_path;
		module_source_codes* const _sources;
		vector<module*> _requirements;
	};
}
