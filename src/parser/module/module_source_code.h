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
	 * \brief source code inside a package
	 */
	struct package_source_code
	{
		enum status
		{
			// the package is not loaded yet
			not_loaded = 0,
			// the package is being loaded
			loading,
			// the package is loaded
			successful,
			// the package failed to load
			failed,
		};

		// the relative path to the source codes
		string_view relative_path;
		// the loading status
		status load_status;
		// sources inside this package
		vector<source_code*> sources;

		~package_source_code()
		{
			for (auto s: sources)
				delete s;
		}
	};

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
		 * \return all source codes part of this package
		 *
		 * this will load the sources found in the supplied directory if not already cached in-memory.
		 */
		virtual package_source_code* get_files(string_view relative_import_path) = 0;
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

		package_source_code* get_files(string_view relative_import_path) final;

#pragma endregion

	private:
		string _root_dir;
		std::unordered_map<string_view, package_source_code*> _sources;

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
		void add(string_view relative_import_path, vector<source_code*> sources);

#pragma region module_source_code_lookup

		package_source_code* get_files(string_view relative_import_path) final;

#pragma endregion

	private:
		std::unordered_map<string_view, package_source_code*> _sources;
	};
}
