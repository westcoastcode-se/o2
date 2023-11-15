//
// Part of the o2 Project, under the Apache License v2.0 with o2 Project Exceptions.
// See the LICENSE file in the project root for license terms
//

#include "node_op_return.h"

using namespace o2;

node_op_return::node_op_return(const source_code_view& view)
		: node_op(view)
{
}

void node_op_return::debug(debug_ostream& stream, int indent) const
{
	stream << this << in(indent);
	stream << "return()" << std::endl;
	node_op::debug(stream, indent);
}
