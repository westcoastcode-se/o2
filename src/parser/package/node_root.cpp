//
// Part of the o2 Project, under the Apache License v2.0 with o2 Project Exceptions.
// See the LICENSE file in the project root for license terms
//

#include "node_root.h"
#include "../module/node_module.h"
#include "../types/node_type.h"

using namespace o2;

void node_root::write_json_properties(json& j)
{
	j.write(json::pair<string_view>{ "type", "root" });
	j.write(json::pair<string_view>{ "id", "/" });

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
}
