//
// Part of the o2 Project, under the Apache License v2.0 with o2 Project Exceptions.
// See the LICENSE file in the project root for license terms
//

#include "node_var.h"
#include <iostream>

using namespace o2;

node_var::node_var(const source_code_view& view, string_view name, int modifiers)
		: node_symbol(view), _name(name), _modifiers(modifiers), _type()
{

}

void node_var::debug(debug_ostream& stream, int indent) const
{
	stream << this << in(indent);
	stream << "var(name=" << _name << ",type=" << _type;
	if (bit_isset(_modifiers, modifier_readonly))
		stream << ",readonly";
	if (bit_isset(_modifiers, modifier_const))
		stream << ",const";
	stream << ")" << std::endl;
	node_symbol::debug(stream, indent);
}

node* node_var::on_child_added(node* n)
{
	const auto nt = dynamic_cast<node_type*>(n);
	if (nt)
		_type = nt;
	return n;
}

void node_var::on_child_removed(node* n)
{
	if (_type == n)
		_type = nullptr;
}

void node_var::on_parent_node(node* p)
{
	if (_type == nullptr)
		throw expected_child_node(get_source_code(), "node_type");
}

void node_var::write_json_properties(json& j)
{
	node_symbol::write_json_properties(j);
	j.write(json::pair<string_view>{ "name", _name });
}

string node_var::get_id() const
{
	stringstream ss;
	const auto symbol = get_parent_of_type<node_symbol>();
	string id;
	if (symbol)
		id = symbol->get_id();
	ss << id;
	if (!id.ends_with('/'))
		ss << '/';
	ss << get_name();
	return std::move(ss.str());
}
