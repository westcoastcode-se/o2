//
// Part of the o2 Project, under the Apache License v2.0 with o2 Project Exceptions.
// See the LICENSE file in the project root for license terms
//

#include "node_type_struct_field.h"

using namespace o2;

node_type_struct_fields::node_type_struct_fields(const source_code_view& view)
		: node(view)
{
	set_query_access_flags(query_access_modifier_passthrough);
}

void node_type_struct_fields::debug(debug_ostream& stream, int indent) const
{
	stream << this << in(indent);
	stream << "type_struct_fields()" << std::endl;
	node::debug(stream, indent);
}

node_type_struct_field::node_type_struct_field(const source_code_view& view, string_view name)
		: node(view), _name(name), _padding(), _field_type()
{
}

void node_type_struct_field::debug(debug_ostream& stream, int indent) const
{
	stream << this << in(indent);
	stream << "type_struct_field(name=" << _name << ",field_type=" << _field_type << ")" << std::endl;
	node::debug(stream, indent);
}

bool node_type_struct_field::resolve(const recursion_detector* rd)
{
	if (!node::resolve(rd))
		return false;
	_field_type = _field_type->get_type();
	return _field_type != nullptr;
}

node* node_type_struct_field::on_child_added(node* n)
{
	const auto type = dynamic_cast<node_type*>(n);
	if (type != nullptr)
		_field_type = type;
	return n;
}

void node_type_struct_field::on_child_removed(node* n)
{
	if (_field_type == n)
		_field_type = nullptr;
}
