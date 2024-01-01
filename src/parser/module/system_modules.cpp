//
// Part of the o2 Project, under the Apache License v2.0 with o2 Project Exceptions.
// See the LICENSE file in the project root for license terms
//

#include "system_modules.h"
#include "module.h"

using namespace o2;

system_modules::system_modules(const std::filesystem::path& compiler_home, syntax_tree* st)
		: _compiler_home(compiler_home), _syntax_tree(st)
{

}

system_modules::~system_modules()
{
	for (auto it: _builtin_modules)
		delete it.second;
}

o2::module* system_modules::find_module(string_view statement)
{
	module* best_match = nullptr;

	const auto it = _builtin_modules.find(statement);
	if (it == _builtin_modules.end())
	{
		// see if there's a directory in the language folder that might have modules in them
		const auto path = _compiler_home / std::filesystem::path(statement);
		if (std::filesystem::is_directory(path))
		{
			best_match = o2_new module(this, string(statement), path.generic_string());
			_builtin_modules[statement] = best_match;
			best_match->insert_into(_syntax_tree);
		}
	}
	else
	{
		best_match = it->second;
	}

	return best_match;
}
