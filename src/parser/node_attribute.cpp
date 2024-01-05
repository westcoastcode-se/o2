//
// Part of the o2 Project, under the Apache License v2.0 with o2 Project Exceptions.
// See the LICENSE file in the project root for license terms
//

#include "node_attribute.h"
#include "struct/node_type_struct.h"

using namespace o2;

namespace
{
	const string_view ATTRIBUTE_ID("stdlib/attribute");
}

node_attribute::node_attribute(const source_code_view& view)
		: node(view), _attribute_type()
{
}

void node_attribute::debug(debug_ostream& stream, int indent) const
{
	stream << this << in(indent);
	stream << "attribute()" << std::endl;
	node::debug(stream, indent);
}

void node_attribute::resolve0(const recursion_detector* rd, resolve_state* state)
{
	if (_attribute_type == nullptr)
		throw expected_child_node(get_source_code(), "node_type");

	node::resolve0(rd, state);
	// TODO verify that the type actually inherits from stdlib.attribute
	_attribute_type = dynamic_cast<node_type_struct*>(_attribute_type->get_type());
	if (_attribute_type == nullptr)
		throw resolve_error_unresolved_reference(get_source_code());
	const auto inherits =
			static_cast<node_type_struct*>(_attribute_type);
	if (inherits->find_inherited_type(ATTRIBUTE_ID) == nullptr)
		throw resolve_error_expected_inherits_from_attribute(get_source_code());
}

node* node_attribute::on_child_added(node* n)
{
	const auto type = dynamic_cast<node_type*>(n);
	if (type != nullptr)
		_attribute_type = type;
	return n;
}

void node_attribute::on_child_removed(node* n)
{
	if (_attribute_type == n)
		_attribute_type = nullptr;
}

node_attributes::node_attributes(const source_code_view& view)
		: node(view)
{
}

void node_attributes::debug(debug_ostream& stream, int indent) const
{
	stream << this << in(indent);
	stream << "attributes()" << std::endl;
	node::debug(stream, indent);
}
