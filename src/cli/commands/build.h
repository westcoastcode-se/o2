//
// Part of the o2 Project, under the Apache License v2.0 with o2 Project Exceptions.
// See the LICENSE file in the project root for license terms
//

#pragma once

#include <cassert>
#include <future>
#include <vector>
#include <string_view>
#include <unordered_map>
#include <atomic>

#include "../channel.h"
#include "../../parser/parser.h"
#include "../../parser/resolver.h"
#include "../../parser/module/module_source_code.h"

namespace o2
{
	/**
	 * \brief state of a specific compilation
	 */
	struct compile_state
	{
		// the state - where you can find things like imports
		parser_state* state;
		// the package we are building
		string_view package_name;
		// all errors that occurred when compiling
		std::vector<string> errors;
		// current import
		string_view import_statement;
		// the resulting package to be added to the syntax tree
		node_package* package;
		// sources to be compiled
		array_view<source_code*> sources;

		explicit compile_state(parser_state* state)
				: state(state), package_name(), errors(), package()
		{
		}

		~compile_state()
		{
			delete state;
		}

		compile_state* init(string_view pkg_name, string_view i, array_view<source_code*> s)
		{
			assert(errors.empty());
			package_name = pkg_name;
			import_statement = i;
			package = nullptr;
			sources = s;
			return this;
		}
	};

	/**
	 * \brief the build command
	 */
	class build
	{
	public:
		struct config
		{
			// Root path where the builder starts running
			string_view path;
			// Path to where the built-in language modules can be found
			string_view lang_path;
			// How much should the builder print out in the terminal
			int verbose_level;
		};

		build(config cfg);

		~build();

		/**
		 * \brief execute the build command
		 * \return
		 */
		int execute();

	private:
		/**
		 * \brief push more items to be built in worker threads
		 * \param ni
		 * \param cs
		 * \return
		 */
		void do_import(compile_state* cs);

		/**
		 * \brief get all source codes found
		 * \param ni
		 * \return
		 */
		array_view<source_code*> get_sources(string_view import_statement, string_view* package_name, bool* loaded);

		/**
		 * \brief parse the source code associated with the compile state
		 * \param cs
		 * \return
		 */
		static compile_state* parse(compile_state* cs);

	private:
		const config _config;
		syntax_tree _syntax_tree;
		channel<compile_state*> _channel;
		int _import_statements_running;

		// TODO add support for a smarted standard lang module imports
		std::unordered_map<string_view, node_module*> _builtin_modules;
		module* _main_module;
	};
}