//
// Part of the o2 Project, under the Apache License v2.0 with o2 Project Exceptions.
// See the LICENSE file in the project root for license terms
//

#include "node_type_reference_of.h"
#include "node_type_array.h"

using namespace o2;

string node_type_reference_of::get_id() const
{
	stringstream ss;
	ss << "(*" << _reference_of->get_id() << ")";
	return std::move(ss.str());
}

void node_type_reference_of::debug(debug_ostream& stream, int indent) const
{
	stream << this << in(indent);
	stream << "type_reference_of(node_type=" << _reference_of << ')' << std::endl;
	node_type::debug(stream, indent);
}

compatibility node_type_reference_of::is_compatible_with(node_type* rhs) const
{
	if (rhs == this)
		return compatibility::identical;
	return node_type::is_compatible_with(rhs);
}

node* node_type_reference_of::on_child_added(node* n)
{
	const auto pointer_of = dynamic_cast<node_type*>(n);
	if (pointer_of)
		_reference_of = pointer_of;
	return n;
}

void node_type_reference_of::on_child_removed(node* n)
{
	if (_reference_of == n)
		_reference_of = nullptr;
}
