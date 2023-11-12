//
// Part of the o2 Project, under the Apache License v2.0 with o2 Project Exceptions.
// See the LICENSE file in the project root for license terms
//

#pragma once

#include "../source_code.h"
#include "../collections/vector.h"
#include <unordered_map>

namespace o2
{
	/**
	 * \brief source code contained inside a module
	 */
	class module_source_codes
	{
	public:
		virtual ~module_source_codes() = default;

		/**
		 * \brief get all sources found at the supplied import path
		 * \param relative_import_path relative path to a directory where one or more sources can be found
		 * \param loaded this will be set to true if new files are loaded
		 * \return a list of all sources for this path
		 *
		 * this will load the sources found in the supplied directory if not already cached in-memory.
		 *
		 * For example: importing "github.com/westcoastcode-se/parser/crypto" till load all files in the
		 * "parser/crypto" directory inside the "github.com/westcoastcode-se" module. It might also
		 * load all files in the "crypto" directory inside the "github.com/westcoastcode-se/parser" module. It depends
		 * on your requirements
		 */
		virtual array_view<source_code*> get_files(string_view relative_import_path, bool* loaded) = 0;
	};

	/**
	 * \brief filesystem implementation for module_source_codes
	 */
	class filesystem_module_source_codes final
			: public module_source_codes
	{
	public:
		explicit filesystem_module_source_codes(string root_dir);

		explicit filesystem_module_source_codes(string_view root_dir);

		~filesystem_module_source_codes() final;

#pragma region module_source_code_lookup

		array_view<source_code*> get_files(string_view relative_import_path, bool* loaded) final;

#pragma endregion

	private:
		/**
		 * \brief load source code files for the supplied import_path
		 * \param import_path
		 * \return
		 */
		array_view<source_code*> load(string_view import_path, bool* loaded);

	private:
		string _root_dir;
		std::unordered_map<string_view, vector<source_code*>> _sources;

		struct entry
		{
			string _text;
			string _filename;
		};
		vector<entry*> _all_source;
	};

	/**
	 * \brief an in-memory implementation for the module_source_codes
	 */
	class memory_module_source_codes final
			: public module_source_codes
	{
	public:
		~memory_module_source_codes() final;

		/**
		 * \brief manually add a list of sources
		 * \param import_path
		 * \param sources the sources
		 */
		void add(string_view import_path, vector<source_code*> sources);

#pragma region module_source_code_lookup

		array_view<source_code*> get_files(string_view import_path, bool* loaded) final;

#pragma endregion

	private:
		std::unordered_map<string_view, vector<source_code*>> _sources;
	};
}
