//
// Part of the o2 Project, under the Apache License v2.0 with o2 Project Exceptions.
// See the LICENSE file in the project root for license terms
//

#include "node_func_parameters.h"

using namespace o2;

node_func_parameters::node_func_parameters(const source_code_view& view)
		: node(view)
{
	set_query_access_flags(query_access_modifier_passthrough);
}

void node_func_parameters::debug(debug_ostream& stream, int indent) const
{
	stream << this << in(indent);
	stream << "arguments(count=" << num_arguments() << ")" << std::endl;
	node::debug(stream, indent);
}

int node_func_parameters::num_arguments() const
{
	return get_children().size();
}

node* node_func_parameters::on_child_added(node* n)
{
	const auto named = dynamic_cast<node_var*>(n);
	if (named)
		_arguments.add(named);
	return n;
}

void node_func_parameters::on_child_removed(node* n)
{
	const auto named = dynamic_cast<node_var*>(n);
	if (named)
	{
		const auto idx = _arguments.find(named);
		if (idx != -1)
			_arguments.remove_at(idx);
	}
}
