//
// Part of the o2 Project, under the Apache License v2.0 with o2 Project Exceptions.
// See the LICENSE file in the project root for license terms
//

#include "node_package.h"
#include <iostream>

using namespace o2;

node_package* node_package::find_package(string_view name) const
{
	auto it = _packages.find(name);
	if (it != _packages.end())
		return it->second;
	return nullptr;
}

node* node_package::on_child_added(node* n)
{
	const auto p = dynamic_cast<node_package*>(n);
	if (p != nullptr)
	{
		// if package already exist then merge the package's content and remove one we are
		// trying to add
		for (auto child: get_children())
		{
			const auto n2 = dynamic_cast<node_package*>(child);
			if (n2 == nullptr)
				continue;

			if (n2 == p)
				break; // already added

			if (n2->get_name() == p->get_name())
			{
				n2->add_children(p->get_children());
				assert(p->get_children().empty());
				delete p;
				return n2;
			}
		}

		_packages[p->_name] = p;
	}
	else
	{
		const auto nv = dynamic_cast<node_var*>(n);
		if (nv != nullptr)
		{
			if (_variables.contains(nv->get_name()))
				throw error_named_symbol_already_declared(n->get_source_code(), nv->get_name());
			_variables[nv->get_name()] = nv;
		}
	}
	return n;
}

void node_package::on_child_removed(node* n)
{
	const auto p = dynamic_cast<node_package*>(n);
	if (p != nullptr)
		_packages.erase(p->_name);
	else
	{
		const auto nv = dynamic_cast<node_var*>(n);
		if (nv != nullptr)
			_variables.erase(nv->get_name());
	}
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
