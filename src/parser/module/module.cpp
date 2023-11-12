//
// Part of the o2 Project, under the Apache License v2.0 with o2 Project Exceptions.
// See the LICENSE file in the project root for license terms
//

#include "module.h"
#include "module_source_code.h"

using namespace o2;

module::~module()
{
	delete _sources;
}

string_view module::get_relative_path(string_view path) const
{
	return path.substr(_name.length());
}

int module::matches(string_view import_path) const
{
	if (_name.size() > import_path.size())
		return -1;
	if (!import_path.starts_with(_name))
		return -1;
	return (int)(import_path.size() - _name.size());
}

array_view<source_code*> module::imported_files(string_view import_path, bool* loaded) const
{
	// given the example of import_path = "westcoastcode.se/compiler/crypto" and the
	// module is "westcoastcode.se/compiler" then the relative path is "/compiler"
	return _sources->get_files(get_relative_path(import_path), loaded);
}
