//
// Part of the o2 Project, under the Apache License v2.0 with o2 Project Exceptions.
// See the LICENSE file in the project root for license terms
//

#include "node_type_ref.h"
#include <iostream>

using namespace o2;

node_type_ref::node_type_ref(const source_code_view& view)
		: node_type(view), _type()
{
	add_phases_left(phase_resolve_size);
}

void node_type_ref::debug(debug_ostream& stream, int indent) const
{
	stream << this << in(indent);
	stream << "type_ref(type=";
	if (_type)
		stream << _type;
	else
		stream << "?";
	stream << ")" << std::endl;
	node_type::debug(stream, indent);
}

void node_type_ref::resolve0(const recursion_detector* rd, resolve_state* state)
{
	node::resolve0(rd, state);

	// assume that the first child is a reference
	const auto ref = dynamic_cast<node_ref*>(get_child(0));
	if (ref == nullptr)
		throw expected_child_node(get_source_code(), "node_ref");

	// collect the results and fetch the best matched one!
	// TODO: add support for fetching the best match instead of just the first
	const auto results = ref->get_result();
	if (results.size() > 1)
		throw resolve_error_multiple_refs(get_source_code());
	_type = dynamic_cast<node_type*>(results[0]);
	if (_type == nullptr)
		throw resolve_error_unresolved_reference(get_source_code());
}

string node_type_ref::get_id() const
{
	if (_type)
		return _type->get_id();
	return {};
}

compatibility node_type_ref::is_compatible_with(node_type* rhs) const
{
	if (_type == nullptr)
		throw resolve_error_unresolved_reference(get_source_code());
	return _type->is_compatible_with(rhs);
}

void node_type_ref::on_process_phase(const recursion_detector* rd, resolve_state* state, int phase)
{
	if (phase != phase_resolve_size)
	{
		return;
	}

	assert(_type != nullptr && "this node's resolve0 method should've been called before this");
	const recursion_detector rd0(rd, this);
	_type->process_phase(&rd0, state, phase);
	_size = _type->get_size();
}
