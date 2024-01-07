//
// Part of the o2 Project, under the Apache License v2.0 with o2 Project Exceptions.
// See the LICENSE file in the project root for license terms
//

#include "node_type_static_scope.h"
#include "../struct/node_type_struct_field.h"

using namespace o2;

void node_type_static_scope::debug(debug_ostream& stream, int indent) const
{
	stream << this << in(indent);
	stream << "type_static_scope()" << std::endl;
	node::debug(stream, indent);
}

node* node_type_static_scope::on_child_added(node* n)
{
	const auto vars = dynamic_cast<node_type_static_scope_vars*>(n);
	if (vars != nullptr)
		_vars = vars;
	else
	{
		const auto funcs = dynamic_cast<node_type_static_scope_funcs*>(n);
		if (funcs != nullptr)
			_funcs = funcs;
	}
	return n;
}

void node_type_static_scope::on_child_removed(node* n)
{
	if (_vars == n)
		_vars = nullptr;
	else if (_funcs == n)
		_funcs = nullptr;
}
