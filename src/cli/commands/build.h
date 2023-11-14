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
	struct async_data
	{
		// the state - where you can find things like imports
		parser_state state;
		// the module we are loading
		module* module;
		// the package, inside the module, we are processing
		string_view package_name;
		// all errors that occurred when compiling
		std::vector<string> errors;
		// the resulting package to be added to the syntax tree
		node_package* package;
		// sources to be compiled
		package_source_code* sources;

		explicit async_data(syntax_tree* st)
				: state(st), module(), package_name(), errors(), package(), sources()
		{
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
			// number of threads that's allowed to work with the source code
			int threads_count;
		};

		explicit build(config cfg);

		~build();

		/**
		 * \brief execute the build command
		 * \return
		 */
		int execute();

	private:
		/**
		 * \brief push more items to be built in worker threads
		 * \param data asynchronous data
		 * \param cs
		 * \return true if an import request is put to be processed
		 */
		bool try_import(async_data* data, module* m, string_view package_name, package_source_code* sources);

		/**
		 * \param import_statement
		 * \return the module for the supplied import statement
		 */
		module* get_module(string_view import_statement);

		/**
		 * \brief parse the source code associated with the compile state
		 * \param cs
		 * \return
		 */
		static async_data* parse(async_data* cs);

		/**
		 *
		 * \param data
		 * \param from from what module's point of view is this import in case of private import statements
		 * \param import_statement
		 * \return a list of all futures to be expected
		 */
		std::vector<std::future<async_data*>> try_imports(async_data* data, module* from, string_view import_statement);

	private:
		const config _config;
		syntax_tree _syntax_tree;
		channel<async_data*> _parse_requests;
		channel<async_data*> _parse_responses;
		int _import_statements_running;

		// threads that are doing stuff
		std::vector<std::jthread> _threads;

		// TODO add support for a smarted standard lang module imports
		std::unordered_map<string_view, module*> _builtin_modules;
		module* _main_module;
	};
}