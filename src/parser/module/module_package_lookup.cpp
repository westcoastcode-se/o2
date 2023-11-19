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
	// TODO figure out a better way to make filesystem::path more platform independent.
	//      windows uses wstring and linux-type systems use u8string
	static const filesystem::path::string_type O2_SUFFIX(L".o2");
	static const filesystem::path::string_type WIN_SUFFIX(L"_win32");
	static const filesystem::path::string_type OSX_SUFFIX(L"_osx");
	static const filesystem::path::string_type LINUX_SUFFIX(L"_linux");

	bool accept(const filesystem::path& p)
	{
		std::wstring_view view(p.native());
		int idx = view.find_last_of(L'.');
		if (idx == wstring::npos)
			return false;

		std::wstring_view suffix = view.substr(idx);
		if (suffix != O2_SUFFIX)
			return false;

		// check if files are should be included on this operating system
		view = view.substr(0, idx);
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
		if (view.ends_with(OSX_SUFFIX) || view.ends_with(LINUX_SUFFIX))
			return false;
#elif __APPLE__
		if (view.ends_with(WIN_SUFFIX) || view.ends_with(LINUX_SUFFIX))
			return false;
#elif __linux__
		if (view.ends_with(WIN_SUFFIX) || view.ends_with(OSX_SUFFIX))
			return false;
#endif
		return true;
	}
}

package_source_info* filesystem_module_package_lookup::get_info(string_view relative_import_path)
{
	auto it = _sources.find(relative_import_path);
	if (it != _sources.end())
		return it->second;

	const auto package_sources = new package_source_info{
			relative_import_path,
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
		if (!accept(path))
			continue;

		// read the actual file content into a string
		ifstream f(path);
		std::stringstream ss;
		ss << f.rdbuf();
		std::string const& s = ss.str();

		// TODO: check what type of file this is. It might be ASCII, UTF-8, UTF-16 and various little and big endian
		package_sources->sources.add(new source_code(ss.str(), fe.path().generic_string()));
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
