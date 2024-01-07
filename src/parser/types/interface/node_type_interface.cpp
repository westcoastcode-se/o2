//
// Part of the o2 Project, under the Apache License v2.0 with o2 Project Exceptions.
// See the LICENSE file in the project root for license terms
//

#include "node_type_interface.h"

using namespace o2;

node_type_interface::node_type_interface(const source_code_view& view, string_view name)
		: node_type(view, 0), _name(name)
{
}
