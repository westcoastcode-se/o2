//
// Part of the o2 Project, under the Apache License v2.0 with o2 Project Exceptions.
// See the LICENSE file in the project root for license terms
//

#include "node_type_primitive.h"
#include "../primitive_value.h"

using namespace o2;

namespace
{
	compatibility upcast[(int)primitive_type::count][(int)primitive_type::count] = {
			// unknown
			{ compatibility::incompatible },
			// int8
			{ compatibility::incompatible, compatibility::identical, compatibility::downcast,  compatibility::upcast,
					compatibility::upcast,    compatibility::upcast,    compatibility::upcast,    compatibility::upcast,
					compatibility::upcast,    compatibility::upcast,       compatibility::upcast,       compatibility::downcast,
					compatibility::downcast },
			// uint8
			{ compatibility::incompatible, compatibility::downcast,  compatibility::identical, compatibility::upcast,
					compatibility::upcast,    compatibility::upcast,    compatibility::upcast,    compatibility::upcast,
					compatibility::upcast,    compatibility::upcast,       compatibility::upcast,       compatibility::downcast,
					compatibility::downcast },
			// int16
			{ compatibility::incompatible, compatibility::downcast,  compatibility::downcast,  compatibility::identical,
					compatibility::downcast,  compatibility::upcast,    compatibility::upcast,    compatibility::upcast,
					compatibility::upcast,    compatibility::upcast,       compatibility::upcast,       compatibility::downcast,
					compatibility::downcast },
			// uint16
			{ compatibility::incompatible, compatibility::downcast,  compatibility::downcast,  compatibility::downcast,
					compatibility::identical, compatibility::upcast,    compatibility::upcast,    compatibility::upcast,
					compatibility::upcast,    compatibility::upcast,       compatibility::upcast,       compatibility::downcast,
					compatibility::downcast },
			// int32
			{ compatibility::incompatible, compatibility::downcast,  compatibility::downcast,  compatibility::downcast,
					compatibility::downcast,  compatibility::identical, compatibility::downcast,  compatibility::upcast,
					compatibility::upcast,    compatibility::upcast,       compatibility::upcast,       compatibility::downcast,
					compatibility::downcast },
			// uint32
			{ compatibility::incompatible, compatibility::downcast,  compatibility::downcast,  compatibility::downcast,
					compatibility::downcast,  compatibility::downcast,  compatibility::identical, compatibility::upcast,
					compatibility::upcast,    compatibility::upcast,       compatibility::upcast,       compatibility::downcast,
					compatibility::downcast },
			// int64
			{ compatibility::incompatible, compatibility::downcast,  compatibility::downcast,  compatibility::downcast,
					compatibility::downcast,  compatibility::downcast,  compatibility::downcast,  compatibility::identical,
					compatibility::downcast,  compatibility::downcast,     compatibility::downcast,     compatibility::downcast,
					compatibility::downcast },
			// uint64
			{ compatibility::incompatible, compatibility::downcast,  compatibility::downcast,  compatibility::downcast,
					compatibility::downcast,  compatibility::downcast,  compatibility::downcast,  compatibility::downcast,
					compatibility::identical, compatibility::downcast,     compatibility::downcast,     compatibility::downcast,
					compatibility::downcast },
			// float32
			{ compatibility::incompatible, compatibility::downcast,  compatibility::downcast,  compatibility::downcast,
					compatibility::downcast,  compatibility::downcast,  compatibility::downcast,  compatibility::downcast,
					compatibility::downcast,  compatibility::identical,    compatibility::upcast,       compatibility::downcast,
					compatibility::incompatible },
			// float64
			{ compatibility::incompatible, compatibility::downcast,  compatibility::downcast,  compatibility::downcast,
					compatibility::downcast,  compatibility::downcast,  compatibility::downcast,  compatibility::downcast,
					compatibility::downcast,  compatibility::upcast,     compatibility::identical,    compatibility::downcast,
					compatibility::incompatible },
			// bool
			{ compatibility::incompatible, compatibility::downcast,  compatibility::downcast,  compatibility::downcast,
					compatibility::downcast,  compatibility::downcast,  compatibility::downcast,  compatibility::downcast,
					compatibility::downcast,  compatibility::incompatible, compatibility::incompatible, compatibility::identical,
					compatibility::incompatible },
			// ptr
			{ compatibility::incompatible, compatibility::downcast,  compatibility::downcast,  compatibility::downcast,
					compatibility::downcast,  compatibility::downcast,  compatibility::downcast,  compatibility::downcast,
					compatibility::downcast,  compatibility::incompatible, compatibility::incompatible, compatibility::incompatible,
					compatibility::identical },
	};
}

node_type_primitive::node_type_primitive(const vector<string_view>& names, int size,
		primitive_type pttype,
		llvm::Type* type)
		: node_type(source_code_view(), size), _names(names), _primitive_type(pttype), _llvm_type(type)
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

	// check compatibility between primitives. the first part in the array are the type we want to
	// set from and the second is the destination type
	const auto rhs_primitive = dynamic_cast<node_type_primitive*>(rhs);
	if (rhs_primitive)
		return upcast[(int)rhs_primitive->_primitive_type][(int)_primitive_type];
	return compatibility::incompatible;
}
