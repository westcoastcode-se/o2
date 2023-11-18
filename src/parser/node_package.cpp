//
// Part of the o2 Project, under the Apache License v2.0 with o2 Project Exceptions.
// See the LICENSE file in the project root for license terms
//

#include "node_package.h"
#include "node_import.h"
#include "module/node_module.h"
#include <iostream>

using namespace o2;

node* node_package::on_child_added(node* n)
{
	const auto nv = dynamic_cast<node_var*>(n);
	if (nv != nullptr)
	{
		if (_variables.contains(nv->get_name()))
			throw error_named_symbol_already_declared(n->get_source_code(), nv->get_name());
		_variables[nv->get_name()] = nv;
	}
	return n;
}

void node_package::on_child_removed(node* n)
{
	const auto nv = dynamic_cast<node_var*>(n);
	if (nv != nullptr)
		_variables.erase(nv->get_name());
}

void node_package::debug(debug_ostream& stream, int indent) const
{
	stream << this << in(indent);
	stream << "package(name=" << _name << ")" << std::endl;
	node::debug(stream, indent);
}

void node_package::query(query_node_visitor* visitor, int flags)
{
	visitor->visit(this);

	flags = limit_query_flags(flags);
	query_parents(visitor, flags);
	if (bit_isset(flags, query_flag_downwards))
		flags = bit_set(flags, query_flag_children);
	query_children(visitor, flags);
}

void node_package::resolve_symbol_id()
{
	const auto symbol = get_parent_of_type<node_symbol>();
	if (symbol)
	{
		stringstream ss;
		ss << symbol->get_id();
		ss << '.';
		ss << _name;
		set_id(std::move(ss.str()));
	}
}

void node_package::on_import_added(node_import* i)
{
	_state.parse.pending_imports.add(i);
}

bool node_package::on_import_removed(node_import* i)
{
	_state.parse.pending_imports.remove(i);
	return _state.parse.pending_imports.empty();
}

void node_package::on_resolved_before(node_package* p)
{
	assert(_state.parse.depended_resolves.find(p) == -1 &&
		   "package should not be added twice");
	_state.parse.depended_resolves.add(p);
}

bool node_package::resolve(const recursion_detector* rd)
{
	std::cout << "resolving " << get_parent_of_type<node_module>()->get_name() << "/"
			  << get_name() << std::endl;
	if (!node::resolve(rd))
	{
		return false;
	}

	const auto deps = std::move(_state.parse.depended_resolves);
	const recursion_detector rd0(rd, this);
	for (auto p: deps)
	{
		std::cout << "\t";
		if (!p->resolve(&rd0))
			return false;
	}
	return true;
}
