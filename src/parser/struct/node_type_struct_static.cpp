//
// Part of the o2 Project, under the Apache License v2.0 with o2 Project Exceptions.
// See the LICENSE file in the project root for license terms
//

#include "node_type_struct_static.h"
#include "node_type_struct_field.h"

using namespace o2;

void node_type_struct_static::debug(std::basic_ostream<char>& stream, int indent) const
{
	stream << this << in(indent);
	stream << "type_struct_static()" << std::endl;
	node::debug(stream, indent);
}

node* node_type_struct_static::on_child_added(node* n)
{
	const auto fi = dynamic_cast<fields*>(n);
	if (fi != nullptr)
		_fields = fi;
	else
	{
		const auto fu = dynamic_cast<funcs*>(n);
		if (fu != nullptr)
			_funcs = fu;
	}
	return n;
}

void node_type_struct_static::on_child_removed(node* n)
{
	if (_fields == n)
		_fields = nullptr;
	else if (_funcs == n)
		_funcs = nullptr;
}

void node_type_struct_static::fields::debug(std::basic_ostream<char>& stream, int indent) const
{
	stream << this << in(indent);
	stream << "type_struct_static::fields()" << std::endl;
	node::debug(stream, indent);
}

void node_type_struct_static::funcs::debug(std::basic_ostream<char>& stream, int indent) const
{
	stream << this << in(indent);
	stream << "type_struct_static::funcs()" << std::endl;
	node::debug(stream, indent);
}
