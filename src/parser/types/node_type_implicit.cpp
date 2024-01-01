//
// Part of the o2 Project, under the Apache License v2.0 with o2 Project Exceptions.
// See the LICENSE file in the project root for license terms
//

#include "node_type_implicit.h"
#include "../node_link.h"
#include "../operations/node_op_assign.h"

using namespace o2;

node_type_implicit::node_type_implicit(const source_code_view& view)
		: node_type(view), _type()
{
	add_phases_left(phase_resolve_size);
}

void node_type_implicit::debug(debug_ostream& stream, int indent) const
{
	stream << this << in(indent);
	stream << "type_implicit(type=";
	if (_type)
		stream << _type;
	else
		stream << "?";
	stream << ")" << std::endl;
	node_type::debug(stream, indent);
}

void node_type_implicit::resolve0(const recursion_detector* rd, resolve_state* state)
{
	const auto link = get_first_child_of_type<node_link>();
	if (link == nullptr)
		throw expected_child_node(get_source_code(), "node_link");
	if (link->is_broken())
		throw expected_child_node(get_source_code(), "node_link->node_op");
	const auto op = dynamic_cast<node_op*>(link->get_node());
	if (op == nullptr)
		throw unexpected_child_node(get_source_code(), "node_link->node_op");

	const recursion_detector rd1(rd, this);
	op->process_phase(&rd1, state, phase_resolve);

	_type = op->get_type();
	if (_type == nullptr)
		throw resolve_error_unresolved_reference(get_source_code());
	_type = _type->get_type();
	if (_type == nullptr)
		throw resolve_error_unresolved_reference(get_source_code());

	node::resolve0(rd, state);
}

string node_type_implicit::get_id() const
{
	if (_type == this)
		return {};

	stringstream ss;
	const auto symbol = get_parent_of_type<node_symbol>();
	if (symbol)
		ss << symbol->get_id();
	ss << _type->get_id();
	ss << '.';
	return std::move(ss.str());
}

void node_type_implicit::on_process_phase(const recursion_detector* rd, resolve_state* state, int phase)
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
