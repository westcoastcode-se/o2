//
// Part of the o2 Project, under the Apache License v2.0 with o2 Project Exceptions.
// See the LICENSE file in the project root for license terms
//

#include "node_type_struct.h"
#include "../package/node_package.h"

using namespace o2;

node_type_struct::node_type_struct(const source_code_view& view, string_view name)
		: node_type(view), _name(name), _fields(), _methods(), _static()
{
}

void node_type_struct::debug(debug_ostream& stream, int indent) const
{
	stream << this << in(indent);
	stream << "type_struct(name=" << get_name() << ",size=" << _size << ")" << std::endl;
	node_type::debug(stream, indent);
}

int node_type_struct::resolve_size(const recursion_detector* rd)
{
	if (has_known_size())
		return _size;

	// TODO: Figure out how large a struct's size should be. Might depend on if virtual or not.
	//       structs might also have no fields
	if (_fields == nullptr)
		return 0;

	rd->raise_error(this);

	int size = 0;
	const recursion_detector rd0(rd, this);
	const auto fields = _fields->get_children();
	for (auto n: fields)
	{
		const auto field = dynamic_cast<node_type_struct_field*>(n);
		if (field == nullptr)
			continue;
		size += field->resolve_size(&rd0);
	}
	_size = size;
	return _size;
}

node* node_type_struct::on_child_added(node* n)
{
	const auto fields = dynamic_cast<node_type_struct_fields*>(n);
	if (fields != nullptr)
		_fields = fields;
	else
	{
		const auto methods = dynamic_cast<node_type_struct_methods*>(n);
		if (methods != nullptr)
			_methods = methods;
		else
		{
			const auto s = dynamic_cast<node_type_struct_static*>(n);
			if (s != nullptr)
				_static = s;
		}
	}
	return n;
}

void node_type_struct::on_child_removed(node* n)
{
	if (_fields == n)
		_fields = nullptr;
	else if (_methods == n)
		_methods = nullptr;
	else if (_static == n)
		_static = nullptr;
}

vector<node_type_struct_field*> node_type_struct::get_all_fields() const
{
	// TODO figure out the number of fields based on inherited structures
	const auto num_fields = _fields != nullptr ? _fields->get_children().size() : 0;
	if (num_fields == 0)
		return {};

	vector<node_type_struct_field*> result;
	for (auto child: _fields->get_children())
	{
		const auto field = dynamic_cast<node_type_struct_field*>(child);
		// for now, no other children than field is allowed in the fields structure
		assert(field != nullptr);
		result.add(field);
	}
	return result;
}

string node_type_struct::get_id() const
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

void node_type_struct::on_parent_node(node* p)
{
	test_collision(this);
}

void node_type_struct::write_json_properties(json& j)
{
	node_symbol::write_json_properties(j);
	j.write(json::pair<int>{ "size", _size });
}

bool node_type_struct::compare_with_symbol(const node_type_struct* rhs) const
{
	return get_name() == rhs->get_name();
}
