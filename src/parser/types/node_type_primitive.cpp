//
// Part of the o2 Project, under the Apache License v2.0 with o2 Project Exceptions.
// See the LICENSE file in the project root for license terms
//

#include "node_type_primitive.h"

using namespace o2;

node_type_primitive::node_type_primitive(const vector<string_view>& names, int size,
		llvm::Type* type)
		: node_type(source_code_view(), size), _names(names), _llvm_type(type)
{
}

void node_type_primitive::debug(debug_ostream& stream, int indent) const
{
	stream << this << in(indent);
	stream << "primitive(names=[";
	for (int i = 0; i < _names.size(); ++i)
	{
		if (i > 0)
			stream << ',';
		const auto& name = _names[i];
		stream << name;
	}
	stream << "],size=" << get_size() << ")" << std::endl;
	node::debug(stream, indent);
}

void node_type_primitive::write_json_properties(json& j)
{
	node_symbol::write_json_properties(j);
	j.write(json::pair<int>{ "size", _size });
}

string node_type_primitive::get_id() const
{
	return string(get_name());
}

compatibility node_type_primitive::is_compatible_with(node_type* rhs) const
{
	if (rhs == this)
		return compatibility::identical;
	return compatibility::incompatible;
}
