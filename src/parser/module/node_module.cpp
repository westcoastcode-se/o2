//
// Part of the o2 Project, under the Apache License v2.0 with o2 Project Exceptions.
// See the LICENSE file in the project root for license terms
//

#include "node_module.h"
#include "module.h"

using namespace o2;

void node_module::debug(debug_ostream& stream, int indent) const
{
	stream << this << in(indent);
	stream << "module(name=" << _module->get_name() << ",path=" << _module->get_root_path() << ")" << std::endl;
	node::debug(stream, indent);
}

string_view node_module::get_name() const
{
	return _module->get_name();
}

string_view node_module::get_relative_path(string_view import_path) const
{
	return _module->get_relative_path(import_path);
}
