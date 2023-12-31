//
// Part of the o2 Project, under the Apache License v2.0 with o2 Project Exceptions.
// See the LICENSE file in the project root for license terms
//

#include "parser_state.h"
#include "syntax_tree.h"
#include "node_import.h"

using namespace o2;

void parser_state::add_import(node_import* i)
{
	_imports.add_unique(i);
}

node_type_primitive* parser_state::get_primitive_type(primitive_type type) const
{
	switch (type)
	{
	case primitive_type::int8:
		return dynamic_cast<node_type_primitive*>(_syntax_tree->get_root_package()->get_child(3));
	case primitive_type::uint8:
		return dynamic_cast<node_type_primitive*>(_syntax_tree->get_root_package()->get_child(4));
	case primitive_type::int16:
		return dynamic_cast<node_type_primitive*>(_syntax_tree->get_root_package()->get_child(5));
	case primitive_type::uint16:
		return dynamic_cast<node_type_primitive*>(_syntax_tree->get_root_package()->get_child(6));
	case primitive_type::int32:
		return dynamic_cast<node_type_primitive*>(_syntax_tree->get_root_package()->get_child(7));
	case primitive_type::uint32:
		return dynamic_cast<node_type_primitive*>(_syntax_tree->get_root_package()->get_child(8));
	case primitive_type::int64:
		return dynamic_cast<node_type_primitive*>(_syntax_tree->get_root_package()->get_child(9));
	case primitive_type::uint64:
		return dynamic_cast<node_type_primitive*>(_syntax_tree->get_root_package()->get_child(10));
	case primitive_type::float32:
		return dynamic_cast<node_type_primitive*>(_syntax_tree->get_root_package()->get_child(11));
	case primitive_type::float64:
		return dynamic_cast<node_type_primitive*>(_syntax_tree->get_root_package()->get_child(12));
	case primitive_type::bool_:
		return dynamic_cast<node_type_primitive*>(_syntax_tree->get_root_package()->get_child(2));
	case primitive_type::ptr:
		throw std::runtime_error("primitive not allowed");
	default:
		throw std::runtime_error("unknown primitive_type");
	}
}

node_type_primitive* parser_state::get_primitive_byte() const
{
	return dynamic_cast<node_type_primitive*>(_syntax_tree->get_root_package()->get_child(1));
}

node_type_primitive* parser_state::find_primitive_type(string_view name) const
{
	static const int COUNT = 13;
	static const string_view PRIMITIVES[COUNT] = {
			"void",
			"byte",
			"bool",
			"int8",
			"uint8",
			"int16",
			"uint16",
			"int32",
			"uint32",
			"int64",
			"uint64",
			"float32",
			"float64",
	};
	static const string_view INT("int");
	static const string_view UINT("uint");
	static const string_view FLOAT("float");

	if (name == INT)
		return dynamic_cast<node_type_primitive*>(_syntax_tree->get_root_package()->get_child(7));
	if (name == UINT)
		return dynamic_cast<node_type_primitive*>(_syntax_tree->get_root_package()->get_child(8));
	if (name == FLOAT)
		return dynamic_cast<node_type_primitive*>(_syntax_tree->get_root_package()->get_child(11));

	for (int i = 0; i < COUNT; ++i)
	{
		if (PRIMITIVES[i] == name)
			return dynamic_cast<node_type_primitive*>(_syntax_tree->get_root_package()->get_child(i));
	}
	return nullptr;
}
