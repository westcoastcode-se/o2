//
// Part of the o2 Project, under the Apache License v2.0 with o2 Project Exceptions.
// See the LICENSE file in the project root for license terms
//

#include "node_symbol.h"

using namespace o2;

bool node_symbol::is_allowed(node* n) const
{
	// TODO iterate upwards in order to figure out if this symbol is allowed to be accessed
	return true;
}

void node_symbol::write_json_properties(json& j)
{
	node::write_json_properties(j);
	j.write(json::pair<string_view>{ "id", get_id() });
}

node* node_symbol::on_child_added(node* n)
{
	const auto a = dynamic_cast<node_attributes*>(n);
	if (a)
		_attributes = a;
	return n;
}

void node_symbol::on_child_removed(node* n)
{
	if (_attributes == n)
		_attributes = nullptr;
}
