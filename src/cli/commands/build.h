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
#include <filesystem>

#include "../channel.h"
#include "../../parser/parser.h"
#include "../../parser/resolver.h"
#include "../../parser/module/module_package_lookup.h"
#include "base_command.h"

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
		// all errors that occurred when compiling
		std::vector<string> errors;
		// the resulting package to be added to the syntax tree
		node_package* package;
		// information on the package to be loaded
		package_source_info* package_info;

		explicit async_data(syntax_tree* st)
				: state(st), module(), errors(), package(), package_info()
		{
		}
	};

	/**
	 * \brief the build command
	 */
	class build final
			: public base_command
	{
	public:
		struct config
		{
			// Root path where the builder starts running
			std::filesystem::path path;
			// Path to where the built-in language modules can be found
			std::filesystem::path lang_path;
			// How much should the builder print out in the terminal
			int verbose_level;
			// number of threads that's allowed to work with the source code
			int threads_count;
		};

		explicit build(config cfg);

		~build() final;

		/**
		 * \brief execute the build command
		 * \return
		 */
		int execute();

		/**
		 * \brief abort the execution if the build
		 */
		void abort() final;

	private:
		/**
		 * \brief push more items to be built in worker threads
		 * \param data asynchronous data
		 * \param cs
		 * \return true if an import request is put to be processed
		 */
		bool try_import(async_data* data, module* m, package_source_info* sources);

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

		// Is the build aborted?
		std::atomic_bool _aborted;
	};
}