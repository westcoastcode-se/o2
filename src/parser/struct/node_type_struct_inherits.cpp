//
// Part of the o2 Project, under the Apache License v2.0 with o2 Project Exceptions.
// See the LICENSE file in the project root for license terms
//

#include "node_type_struct_inherits.h"

using namespace o2;

void node_type_struct_inherits::debug(debug_ostream& stream, int indent) const
{
	stream << this << in(indent);
	stream << "type_struct_inherits()" << std::endl;
	node::debug(stream, indent);
}

void node_type_struct_inherits::resolve0(const recursion_detector* rd, resolve_state* state)
{
	node::resolve0(rd, state);
	// TODO: verify that a struct only inherits from one struct
}

node_type* node_type_struct_inherits::find_inherited_type(string_view id) const
{
	for (const auto n: get_children())
	{
		const auto inherit = dynamic_cast<node_type_struct_inherit*>(n);
		if (inherit == nullptr)
			throw unexpected_child_node(get_source_code(), "node_type_struct_inherit");

		const auto type = inherit->get_inherits_from();
		if (type->get_id() == id)
			return type;
	}
	return nullptr;
}

bool node_type_struct_inherits::inherits_from_type(node_type* type) const
{
	assert (!has_phase_left(phase_resolve) &&
			"you are not allowed to call this function until after the resolve phase is done for this object");

	for (const auto n: get_children())
	{
		const auto inherit = dynamic_cast<node_type_struct_inherit*>(n);
		if (inherit == nullptr)
			throw unexpected_child_node(get_source_code(), "node_type_struct_inherit");

		const auto t = inherit->get_inherits_from();
		if (t == type)
			return true;
	}

	return false;
}

void node_type_struct_inherit::debug(debug_ostream& stream, int indent) const
{
	stream << this << in(indent);
	stream << "type_struct_inherit(";
	stream << "inherits_from=" << _inherits_from;
	stream << ")" << std::endl;
	node::debug(stream, indent);
}

void node_type_struct_inherit::resolve0(const recursion_detector* rd, resolve_state* state)
{
	node::resolve0(rd, state);
	_inherits_from = _inherits_from->get_type();
	if (_inherits_from == nullptr)
		throw resolve_error_unresolved_reference(get_source_code());
}

void node_type_struct_inherit::on_process_phase(const recursion_detector* rd, resolve_state* state, int phase)
{
	if (phase != phase_resolve_size)
	{
		return;
	}

	assert(_inherits_from != nullptr && "this node's resolve0 method should've been called before this");

	const recursion_detector rd0(rd, this);
	_inherits_from->process_phase(&rd0, state, phase);
	//_size = _inherits_from->get_size();
}

node* node_type_struct_inherit::on_child_added(node* n)
{
	const auto type = dynamic_cast<node_type*>(n);
	if (type != nullptr)
		_inherits_from = type;
	return n;
}

void node_type_struct_inherit::on_child_removed(node* n)
{
	if (_inherits_from == n)
		_inherits_from = nullptr;
}
