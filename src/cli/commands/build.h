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
#include "../../parser/module/module_package_lookup.h"
#include "base_command.h"

namespace o2
{
	/**
	 * \brief state of a specific compilation
	 */
	struct async_data
	{
		// data sent into the processor
		struct _in
		{
			// the module where the source code is loaded from
			module* module;
			// information on the package sources to be loaded
			package_source_info* package_info;
		} in;

		// data coming out from the processor
		struct _out
		{
			// the state - where you can find things like imports
			parser_state state;
			// all errors that occurred when compiling
			std::vector<string> errors;
			// the resulting package to be added to the syntax tree
			node_package* package;
		} out;

		explicit async_data(syntax_tree* st)
				: in({ nullptr, nullptr }),
				  out({ parser_state(st), {}, nullptr })
		{
		}
	};

	enum class build_config_output
	{
		json,
		binary,
		debug
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
			// what are we outputting
			build_config_output output_type;
			// the destination where to put the result into
			string_view output_destination;
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
		 * \param data asynchronous data that can be associated with this import
		 * \param import_request the import where this request originates
		 * \param imported_module the module where the source code can be found in
		 * \param package_info information on the source code to be loaded
		 * \return true if an import request is being processed
		 * \remark if the return value is true and the supplied data is not nullptr, then the ownership of
		 *         that memory is moved to the processor.
		 */
		package_source_info::status try_import(async_data* data, node_import* import_request, module* imported_module,
				package_source_info* package_info);

		/**
		 * \brief parse the source code associated with the compile state
		 * \param data
		 * \return
		 */
		static async_data* parse(async_data* data);

		/**
		 * \brief print out json output
		 */
		int output_json();

	private:
		const config _config;
		llvm::LLVMContext _context;
		syntax_tree _syntax_tree;

		int _pending_requests;
		channel<async_data*> _parse_requests;
		channel<async_data*> _parse_responses;

		// threads that are doing stuff
		std::vector<std::jthread> _threads;

		system_modules _system_module;
		module* _main_module;

		// Is the build aborted?
		std::atomic_bool _aborted;
	};
}