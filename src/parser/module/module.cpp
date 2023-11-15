//
// Part of the o2 Project, under the Apache License v2.0 with o2 Project Exceptions.
// See the LICENSE file in the project root for license terms
//

#include "module.h"
#include "module_package_lookup.h"
#include "../syntax_tree.h"

using namespace o2;

module::~module()
{
	delete _sources;
	// cleanup the node_module if it's not added to the syntax tree
	if (!bit_isset(_modifiers, modifier_added))
		delete _node_module;
}

string_view module::get_relative_path(string_view import_path) const
{
	return import_path.substr(_name.length());
}

int module::matches(string_view import_path) const
{
	if (_name.size() >= import_path.size())
		return MATCHES_NO_MATCH;
	if (!import_path.starts_with(_name))
		return MATCHES_NO_MATCH;
	return (int)(import_path.size() - _name.size());
}

package_source_info* module::get_package_info(string_view import_path) const
{
	return _sources->get_info(get_relative_path(import_path));
}

void module::load_package_sources(package_source_info* info) const
{
	_sources->load_sources(info);
}

node_module* module::insert_into(syntax_tree* st)
{
	assert(!bit_isset(_modifiers, modifier_added));
	st->get_root_package()->add_child(_node_module);
	_modifiers = bit_set(_modifiers, modifier_added);
	return _node_module;
}

node_package* module::add_package(node_package* p)
{
	assert(bit_isset(_modifiers, modifier_added));
	_node_module->add_child(p);
	return p;
}
