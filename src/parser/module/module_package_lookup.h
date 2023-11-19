//
// Part of the o2 Project, under the Apache License v2.0 with o2 Project Exceptions.
// See the LICENSE file in the project root for license terms
//

#pragma once

#include "../source_code.h"
#include "../collections/vector.h"
#include <unordered_map>
#include <filesystem>

namespace o2
{
	/**
	 * \brief info about the source code inside a package
	 */
	struct package_source_info
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

		// relative name of the package where this data is located
		string_view name;
		// the relative path to the source codes
		std::filesystem::path relative_path;
		// the loading status
		status load_status;
		// sources inside this package
		vector<source_code*> sources;

		~package_source_info()
		{
			for (auto s: sources)
				delete s;
		}
	};

	/**
	 * \brief a lookup type that can be used to get sources associated with a specific package inside a module
	 */
	class module_package_lookup
	{
	public:
		virtual ~module_package_lookup() = default;

		/**
		 * \brief get the information of a relative path inside a module
		 * \param relative_import_path relative path to a directory where one or more sources can be found
		 * \return information on the sources found inside the package
		 */
		virtual package_source_info* get_info(string_view relative_import_path) = 0;

		/**
		 * \brief load sources for the supplied information
		 * \param info the package source information
		 */
		virtual void load_sources(package_source_info* info) const
		{
		}
	};

	/**
	 * \brief filesystem implementation for module_source_codes
	 */
	class filesystem_module_package_lookup final
			: public module_package_lookup
	{
	public:
		explicit filesystem_module_package_lookup(const std::filesystem::path& root_dir);

		explicit filesystem_module_package_lookup(string_view root_dir);

		~filesystem_module_package_lookup() final;

#pragma region module_source_code_lookup

		package_source_info* get_info(string_view relative_import_path) final;

		void load_sources(package_source_info* package_sources) const final;

#pragma endregion

	private:
		std::filesystem::path _root_dir;
		std::unordered_map<string_view, package_source_info*> _sources;
	};

	/**
	 * \brief an in-memory implementation for the module_source_codes
	 */
	class memory_module_package_lookup final
			: public module_package_lookup
	{
	public:
		~memory_module_package_lookup() final;

		/**
		 * \brief manually add a list of sources
		 * \param import_path
		 * \param sources the sources
		 */
		void add(string_view relative_import_path, vector<source_code*> sources);

#pragma region module_source_code_lookup

		package_source_info* get_info(string_view relative_import_path) final;

#pragma endregion

	private:
		std::unordered_map<string_view, package_source_info*> _sources;
	};
}
