//
// Part of the o2 Project, under the Apache License v2.0 with o2 Project Exceptions.
// See the LICENSE file in the project root for license terms
//

#include "module.h"
#include "module_package_lookup.h"
#include "../syntax_tree.h"
#include "../node_import.h"

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

void module::notify_package_imported(node_package* p)
{
	// potentially imports that's loaded
	for(int idx = _state.parse.import_requests.size() - 1; idx >= 0; --idx)
	{
		const auto i = _state.parse.import_requests[idx];
		const auto relative_package_name = get_relative_path(i->get_import_statement());
		if (p->get_name() == relative_package_name)
		{
			// this is not imported, so we don't need to keep track of it anymore
			_state.parse.import_requests.remove_at(idx);
			if (i->notify_imported())
			{
				// the package where this import is part of can now be resolved. so add it to this module as a
				// pending package to be resolved
				p->on_resolved_before(i->get_parent_of_type<node_package>());
			}
		}
	}
}

void module::add_import_request(node_import* i)
{
	_state.parse.import_requests.add(i);
}
