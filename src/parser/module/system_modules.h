//
// Part of the o2 Project, under the Apache License v2.0 with o2 Project Exceptions.
// See the LICENSE file in the project root for license terms
//

#pragma once

#include "module_package_lookup.h"
#include "../source_code.h"
#include "node_module.h"
#include "../syntax_tree.h"
#include <filesystem>

namespace o2
{
	class module;

	/**
	 * \brief a special module that represents all built-in system modules, such as stdlib
	 */
	class system_modules
	{
	public:
		/**
		 * \param compiler_home path to where the actual compiler is located
		 * \param st the main syntax tree
		 */
		system_modules(const std::filesystem::path& compiler_home, syntax_tree* st);

		~system_modules();

		/**
		 * \brief search for a required module in this module
		 * \param statement the import statement
		 * \return
		 */
		[[nodiscard]] module* find_module(string_view statement);

	private:
		const std::filesystem::path _compiler_home;
		syntax_tree* const _syntax_tree;
		std::unordered_map<string_view, module*> _builtin_modules;
	};
}