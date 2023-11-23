//
// Part of the o2 Project, under the Apache License v2.0 with o2 Project Exceptions.
// See the LICENSE file in the project root for license terms
//

#include "json_serializable.h"

using namespace o2;

void json_serializable::write_json(json& j)
{
	auto child_json = j.write(json::object{});
	write_json_properties(child_json);
}

void json_serializable::write_json_properties(json& j)
{
	j.write(json::pair<string_view>{ "type", get_json_type() });
}
