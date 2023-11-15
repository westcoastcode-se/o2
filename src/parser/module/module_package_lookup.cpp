//
// Part of the o2 Project, under the Apache License v2.0 with o2 Project Exceptions.
// See the LICENSE file in the project root for license terms
//

#include "module_package_lookup.h"
#include <filesystem>
#include <fstream>
#include <sstream>
#include <utility>

using namespace o2;
using namespace std;

filesystem_module_package_lookup::filesystem_module_package_lookup(const std::filesystem::path& root_dir)
		: _root_dir(root_dir)
{
}

filesystem_module_package_lookup::filesystem_module_package_lookup(string_view root_dir)
		: _root_dir(root_dir)
{
}

filesystem_module_package_lookup::~filesystem_module_package_lookup()
{
	for (auto s: _sources)
		delete s.second;
}

namespace
{
	static const filesystem::path O2_SUFFIX(".o2");
}

package_source_info* filesystem_module_package_lookup::get_info(string_view relative_import_path)
{
	auto it = _sources.find(relative_import_path);
	if (it != _sources.end())
		return it->second;

	const auto package_sources = new package_source_info{
			relative_import_path,
			package_source_info::not_loaded
	};
	_sources[relative_import_path] = package_sources;
	return package_sources;
}

void filesystem_module_package_lookup::load_sources(package_source_info* package_sources) const
{
	const std::filesystem::path p = _root_dir / package_sources->relative_path.relative_path();
	for (const auto& fe: filesystem::directory_iterator(p))
	{
		if (!fe.is_regular_file())
			continue;
		// is this a source code file?
		const auto& path = fe.path();
		if (path.extension() != O2_SUFFIX)
			continue;
		ifstream f(path);
		package_sources->sources.add(new source_code(
				std::move(string(istreambuf_iterator<char>(f), istreambuf_iterator<char>())),
				fe.path().generic_string()));
		f.close();
	}
}

memory_module_package_lookup::~memory_module_package_lookup()
{
	for (const auto& pair: _sources)
		delete pair.second;
}

void memory_module_package_lookup::add(string_view import_path, vector<source_code*> sources)
{
	_sources[import_path] = new package_source_info{
			import_path,
			package_source_info::not_loaded,
			std::move(sources)
	};
}

package_source_info* memory_module_package_lookup::get_info(string_view relative_import_path)
{
	auto it = _sources.find(relative_import_path);
	if (it == _sources.end())
		return {};
	return it->second;
}
