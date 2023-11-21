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
	stream << "package(name=" << get_name();
	if (bit_isset(_status, status_resolved))
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

bool node_package::resolve(const recursion_detector* rd)
{
	if (!node::resolve(rd))
		return false;

	// resolve all dependencies that are waiting for this package to be resolved
	const auto deps = std::move(_state.parse.depended_resolves);
	const recursion_detector rd0(rd, this);
	for (auto p: deps)
	{
		if (!p->resolve(&rd0))
			return false;
	}
	_status = status_resolved;
	return true;
}

void node_package::write_json_properties(json& j)
{
	j.write(json::pair<string_view>{ "type", "package" });
	node_symbol::write_json_properties(j);

	// add modules
	{
		auto module_json = j.write(json::array{ "modules" });
		for (auto module: get_children_of_type<node_module>())
		{
			module->write_json(module_json);
		}
	}

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
	test_collision(this);
}

string node_package::get_id() const
{
	stringstream ss;
	const auto symbol = get_parent_of_type<node_symbol>();
	if (symbol)
		ss << symbol->get_id();
	ss << '/';
	ss << _name;
	return std::move(ss.str());
}

bool node_package::compare_with_symbol(const node_package* rhs) const
{
	return get_name() == rhs->get_name();
}
