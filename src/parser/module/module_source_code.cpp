//
// Part of the o2 Project, under the Apache License v2.0 with o2 Project Exceptions.
// See the LICENSE file in the project root for license terms
//

#include "module_source_code.h"
#include <filesystem>
#include <fstream>
#include <sstream>
#include <utility>

using namespace o2;
using namespace std;

filesystem_module_source_codes::filesystem_module_source_codes(string root_dir)
		: _root_dir(std::move(root_dir))
{
}

filesystem_module_source_codes::filesystem_module_source_codes(string_view root_dir)
		: _root_dir(root_dir)
{
}

filesystem_module_source_codes::~filesystem_module_source_codes()
{
	for (auto s: _all_source)
		delete s;
}

namespace
{
	static const filesystem::path O2_SUFFIX(".o2");
}

package_source_code* filesystem_module_source_codes::get_files(string_view relative_import_path)
{
	auto it = _sources.find(relative_import_path);
	if (it != _sources.end())
		return it->second;

	stringstream ss;
	ss << _root_dir << relative_import_path;
	vector<source_code*> sources;
	for (const auto& fe: filesystem::directory_iterator(ss.str()))
	{
		if (!fe.is_regular_file())
			continue;
		// is this a source code file?
		const auto& path = fe.path();
		if (path.extension() != O2_SUFFIX)
			continue;
		ifstream f(path);
		const auto one_of_them = new entry{
				std::move(string(istreambuf_iterator<char>(f), istreambuf_iterator<char>())),
				fe.path().generic_string()
		};
		_all_source.add(one_of_them);
		sources.add(new source_code(one_of_them->_text, one_of_them->_filename));
	}

	const auto package_sources = new package_source_code{
			relative_import_path,
			package_source_code::not_loaded,
			std::move(sources)
	};
	_sources[relative_import_path] = package_sources;
	return package_sources;
}

memory_module_source_codes::~memory_module_source_codes()
{
	for (const auto& pair: _sources)
		delete pair.second;
}

void memory_module_source_codes::add(string_view import_path, vector<source_code*> sources)
{
	_sources[import_path] = new package_source_code{
			import_path,
			package_source_code::not_loaded,
			std::move(sources)
	};
}

package_source_code* memory_module_source_codes::get_files(string_view relative_import_path)
{
	auto it = _sources.find(relative_import_path);
	if (it == _sources.end())
		return {};
	return it->second;
}
