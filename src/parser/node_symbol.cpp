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
