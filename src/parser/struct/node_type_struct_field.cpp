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
		: node_symbol(view), _name(name), _padding(), _size(-1), _field_type()
{
	add_phases_left(phase_resolve_size);
}

void node_type_struct_field::debug(debug_ostream& stream, int indent) const
{
	stream << this << in(indent);
	stream << "type_struct_field(name=" << _name << ",field_type=" << _field_type << ")" << std::endl;
	node::debug(stream, indent);
}

void node_type_struct_field::resolve0(const recursion_detector* rd, resolve_state* state)
{
	node::resolve0(rd, state);
	_field_type = _field_type->get_type();
	if (_field_type == nullptr)
		throw resolve_error_unresolved_reference(get_source_code());
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

string node_type_struct_field::get_id() const
{
	stringstream ss;
	const auto symbol = get_parent_of_type<node_symbol>();
	if (symbol)
		ss << symbol->get_id();
	ss << '/';
	ss << get_name();
	return std::move(ss.str());
}

bool node_type_struct_field::compare_with_symbol(const node_type_struct_field* rhs) const
{
	return get_name() == rhs->get_name();
}

void node_type_struct_field::on_parent_node(node* p)
{
	test_collision(this);
}

void node_type_struct_field::on_process_phase(const recursion_detector* rd, resolve_state* state, int phase)
{
	if (phase != phase_resolve_size)
	{
		return;
	}

	assert(_field_type != nullptr && "this node's resolve0 method should've been called before this");

	const recursion_detector rd0(rd, this);
	_field_type->process_phase(&rd0, state, phase);
	_size = _field_type->get_size();
}
