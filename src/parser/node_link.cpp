//
// Part of the o2 Project, under the Apache License v2.0 with o2 Project Exceptions.
// See the LICENSE file in the project root for license terms
//

#include "node_link.h"

using namespace o2;

node_link::~node_link()
{
	if (_link)
		_link->_link = nullptr;
	_link = nullptr;
}

node* node_link::get_node() const
{
	if (_link)
		return _link->get_parent();
	return nullptr;
}

node_link* node_link::new_link()
{
	assert(_link == nullptr);
	_link = o2_new node_link(get_source_code());
	_link->_link = this;
	return _link;
}

void node_link::debug(debug_ostream& stream, int indent) const
{
	stream << this << in(indent);
	stream << "link(node=" << _link << ")" << std::endl;
	node::debug(stream, indent);
}
