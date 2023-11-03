//
// Part of the o2 Project, under the Apache License v2.0 with o2 Project Exceptions.
// See the LICENSE file in the project root for license terms
//

#include "node_module.h"

using namespace o2;

node_module::~node_module()
{
	delete _module;
}

void node_module::debug(std::basic_ostream<char>& stream, int indent) const
{
	stream << this << in(indent);
	stream << "module(name=" << _module->get_name() << ",path=" << _module->get_root_path() << ")" << std::endl;
	node::debug(stream, indent);
}

bool node_module::resolve(const recursion_detector* rd)
{
	return node::resolve(rd);
}

void node_module::query(query_node_visitor* visitor, int flags)
{
	node::query(visitor, flags);
}
