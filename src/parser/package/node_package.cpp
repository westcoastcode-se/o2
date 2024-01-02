//
// Part of the o2 Project, under the Apache License v2.0 with o2 Project Exceptions.
// See the LICENSE file in the project root for license terms
//

#include "node_package.h"
#include "../node_import.h"
#include "../module/node_module.h"
#include "../functions/node_func.h"
#include <iostream>

using namespace o2;

node* node_package::on_child_added(node* n)
{
	const auto nv = dynamic_cast<node_var*>(n);
	if (nv != nullptr)
	{
		if (_variables.contains(nv->get_name()))
			throw error_named_symbol_already_declared(get_source_code(), get_name(), n->get_source_code());
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
	stream << "package(name=" << get_name();
	if (!has_phase_left(phase_resolve))
		stream << ",resolved";
	stream << ")" << std::endl;
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

void node_package::process_phases()
{
	const recursion_detector rd;
	const recursion_detector rd0(&rd, this);
	resolve_state state(this);

	process_phase_resolve(&rd0, &state);
	process_phase_resolve_size(&rd0, &state);
	// TODO: Add more phases
}

void node_package::write_json_properties(json& j)
{
	node_symbol::write_json_properties(j);

	// add types
	{
		auto types_json = j.write(json::array{ "types" });
		for (auto type: get_children_of_type<node_type>())
		{
			type->write_json(types_json);
		}
	}

	// add funcs
	{
		auto functions_json = j.write(json::array{ "functions" });
		for (auto func: get_children_of_type<node_func>())
		{
			func->write_json(functions_json);
		}
	}
}

void node_package::on_parent_node(node* p)
{
	superficial_collision_test(this);
}

string node_package::get_id() const
{
	stringstream ss;
	const auto symbol = get_parent_of_type<node_symbol>();
	if (symbol)
		ss << symbol->get_id();
	if (!_name.empty())
	{
		assert(_name[0] == '/');
		ss << _name;
	}
	return std::move(ss.str());
}

bool node_package::superficial_test_symbol_collision(const node_package* rhs) const
{
	if (get_name() == rhs->get_name())
		throw error_named_symbol_already_declared(get_source_code(), get_name(), rhs->get_source_code());
	return false;
}

void node_package::process_phase_resolve(const recursion_detector* rd, resolve_state* state)
{
	// Start by resolving all nodes in this package
	for (auto c: get_children())
		c->process_phase(rd, state, node::phase_resolve);

	// Then resolve all dependencies that are waiting for this package to be resolved
	const auto deps = std::move(_state.parse.depended_resolves);
	for (auto p: deps)
		p->process_phase_resolve(rd, state);
}

void node_package::process_phase_resolve_size(const recursion_detector* rd, resolve_state* state)
{
	auto nodes = std::move(state->get_nodes());
	for (auto n: nodes)
		n->process_phase(rd, state, node::phase_resolve_size);
}
