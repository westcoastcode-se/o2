//
// Part of the o2 Project, under the Apache License v2.0 with o2 Project Exceptions.
// See the LICENSE file in the project root for license terms
//

#pragma once

#include "../node.h"

namespace o2
{
	class module;

	/**
	 * \brief class used to connect a module to the syntax tree
	 */
	class node_module
			: public node
	{
	public:
		explicit node_module(module* m)
				: node(source_code_view()), _module(m)
		{
		}

		/**
		 * \return the name of this module
		 */
		string_view get_name() const;

		/**
		 * \param import_path
		 * \return the relative path
		 */
		string_view get_relative_path(string_view import_path) const;

		/**
		 * \return the module
		 */
		module* get_module() const
		{
			return _module;
		}

#pragma region node

		void debug(std::basic_ostream<char>& stream, int indent) const final;

#pragma endregion

	private:
		module* const _module;
	};
}
