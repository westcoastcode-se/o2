//
// Part of the o2 Project, under the Apache License v2.0 with o2 Project Exceptions.
// See the LICENSE file in the project root for license terms
//

#include "syntax_tree.h"
#include "types/node_type_primitive.h"
#include <iostream>

using namespace o2;

syntax_tree::syntax_tree()
		: _root(source_code_view(), "")
{
	// added predefined primitive types
	_root.add_child(o2_new node_type_primitive(vector<string_view>("void"), 0));
	_root.add_child(o2_new node_type_primitive(vector<string_view>("byte"), 1));
	_root.add_child(o2_new node_type_primitive(vector<string_view>("bool"), 4));
	_root.add_child(o2_new node_type_primitive(vector<string_view>("int8"), sizeof(int8_t)));
	_root.add_child(o2_new node_type_primitive(vector<string_view>("uint8"), sizeof(uint8_t)));
	_root.add_child(o2_new node_type_primitive(vector<string_view>("int16"), sizeof(int16_t)));
	_root.add_child(o2_new node_type_primitive(vector<string_view>("uint16"), sizeof(uint16_t)));
	_root.add_child(o2_new node_type_primitive(vector<string_view>("int", "int32"), sizeof(int32_t)));
	_root.add_child(o2_new node_type_primitive(vector<string_view>("uint", "uint32"), sizeof(uint32_t)));
	_root.add_child(o2_new node_type_primitive(vector<string_view>("int64"), sizeof(int64_t)));
	_root.add_child(o2_new node_type_primitive(vector<string_view>("uint64"), sizeof(uint64_t)));
	_root.add_child(o2_new node_type_primitive(vector<string_view>("float", "float32"), sizeof(float)));
	_root.add_child(o2_new node_type_primitive(vector<string_view>("float64"), sizeof(double)));
}

void syntax_tree::debug() const
{
	_root.debug(std::cout, 1);
}

void syntax_tree::resolve()
{
	const recursion_detector rd;
	_root.resolve(&rd);
}

void syntax_tree::optimize(node_optimizer* optimizer)
{
	_root.optimize(optimizer);
}
