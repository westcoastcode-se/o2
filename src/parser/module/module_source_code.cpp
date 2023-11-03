//
// Part of the o2 Project, under the Apache License v2.0 with o2 Project Exceptions.
// See the LICENSE file in the project root for license terms
//

#include "module_source_code.h"
#include <filesystem>
#include <fstream>
#include <sstream>

using namespace o2;
using namespace std;

filesystem_module_source_codes::filesystem_module_source_codes(string root_dir)
		: _root_dir(std::move(root_dir))
{
}

filesystem_module_source_codes::~filesystem_module_source_codes()
{
	for (auto s: _all_source)
		delete s;
}

array_view<source_code*> filesystem_module_source_codes::get_files(string_view import_path, bool* loaded)
{
	auto it = _sources.find(import_path);
	if (it == _sources.end())
		return load(import_path, loaded);
	if (it->second.empty())
		return load(import_path, loaded);
	return it->second;
}

array_view<source_code*> filesystem_module_source_codes::load(string_view import_path, bool* loaded)
{
	stringstream ss;
	ss << _root_dir << '/' << import_path;
	entry* one_of_them = nullptr;
	vector<source_code*> sources;
	for (const auto& fe: filesystem::directory_iterator(ss.str()))
	{
		if (!fe.is_regular_file())
			continue;
		// is this a source code file?
		const auto path = fe.path();
		if (path.extension() != filesystem::path(".o2"))
			continue;
		ifstream f(path);
		one_of_them = new entry{
				string(istreambuf_iterator<char>(f), istreambuf_iterator<char>()),
				fe.path().generic_string()
		};
		_all_source.add(one_of_them);
		sources.add(new source_code(one_of_them->_text, one_of_them->_filename));
	}

	if (one_of_them == nullptr)
		return {};
	*loaded = true;
	const auto it = _sources.insert(make_pair(import_path, std::move(sources)));
	return it.first->second;
}

memory_module_source_codes::~memory_module_source_codes()
{
	for (const auto& pair: _sources)
	{
		for (const auto s: pair.second)
			delete s;
	}
}

void memory_module_source_codes::add(string_view import_path, vector<source_code*> sources)
{
	_sources[import_path] = sources;
}

array_view<source_code*> memory_module_source_codes::get_files(string_view import_path, bool* loaded)
{
	auto it = _sources.find(import_path);
	if (it == _sources.end())
		return {};
	return it->second;
}
