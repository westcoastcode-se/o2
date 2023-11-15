//
// Part of the o2 Project, under the Apache License v2.0 with o2 Project Exceptions.
// See the LICENSE file in the project root for license terms
//

#include "node_func_body.h"
#include "node_func.h"

using namespace o2;

void node_func_body::debug(debug_ostream& stream, int indent) const
{
	stream << this << in(indent);
	stream << "func_body(def=" << _def << ")" << std::endl;
	node::debug(stream, indent);
}

void node_func_body::on_parent_node(node* n)
{
	const auto func = dynamic_cast<node_func*>(n);
	if (func)
		_def = func;
}

void node_func_body::on_removed_parent_node(node* n)
{
	const auto func = dynamic_cast<node_func*>(n);
	if (func)
		_def = nullptr;
}
