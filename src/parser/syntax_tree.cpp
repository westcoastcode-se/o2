//
// Part of the o2 Project, under the Apache License v2.0 with o2 Project Exceptions.
// See the LICENSE file in the project root for license terms
//

#include "syntax_tree.h"
#include "types/node_type_primitive.h"
#include <iostream>
#include <locale>
#include <codecvt>

using namespace o2;

syntax_tree::syntax_tree()
		: _root(source_code_view(), STR(""))
{
	// added predefined primitive types
	_root.add_child(o2_new node_type_primitive(vector<string_view>(STR("void")), 0));
	_root.add_child(o2_new node_type_primitive(vector<string_view>(STR("byte")), 1));
	_root.add_child(o2_new node_type_primitive(vector<string_view>(STR("bool")), 4));
	_root.add_child(o2_new node_type_primitive(vector<string_view>(STR("int8")), sizeof(int8_t)));
	_root.add_child(o2_new node_type_primitive(vector<string_view>(STR("uint8")), sizeof(uint8_t)));
	_root.add_child(o2_new node_type_primitive(vector<string_view>(STR("int16")), sizeof(int16_t)));
	_root.add_child(o2_new node_type_primitive(vector<string_view>(STR("uint16")), sizeof(uint16_t)));
	_root.add_child(o2_new node_type_primitive(vector<string_view>(STR("int"), STR("int32")), sizeof(int32_t)));
	_root.add_child(o2_new node_type_primitive(vector<string_view>(STR("uint"), STR("uint32")), sizeof(uint32_t)));
	_root.add_child(o2_new node_type_primitive(vector<string_view>(STR("int64")), sizeof(int64_t)));
	_root.add_child(o2_new node_type_primitive(vector<string_view>(STR("uint64")), sizeof(uint64_t)));
	_root.add_child(o2_new node_type_primitive(vector<string_view>(STR("float"), STR("float32")), sizeof(float)));
	_root.add_child(o2_new node_type_primitive(vector<string_view>(STR("float64")), sizeof(double)));
}

void syntax_tree::debug() const
{
	stringstream ss;
	_root.debug(ss, 1);
	string output = std::move(ss.str());
	for (const auto c: output)
		std::wcout << wchar_t(c);
	std::wcout << std::endl;
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
