//
// Part of the o2 Project, under the Apache License v2.0 with o2 Project Exceptions.
// See the LICENSE file in the project root for license terms
//

#include "node_type_complex.h"
#include "../../package/node_package.h"

using namespace o2;

node_type_complex::node_type_complex(const source_code_view& view, string_view name)
	: node_type(view), _name(name), _type(complex_type::unknown_), _inherits(), _fields(), _methods(), _static()
{
	add_phases_left(phase_resolve_size);
}

void node_type_complex::debug(debug_ostream& stream, int indent) const
{
	stream << this << in(indent);
	stream << "type_struct(name=" << get_name() << ",type=";
	switch (_type)
	{
	case complex_type::struct_:
		stream << "struct";
		break;
	case complex_type::class_:
		stream << "class";
		break;
	case complex_type::interface_:
		stream << "interface";
		break;
	case complex_type::trait_:
		stream << "trait";
		break;
	case complex_type::unknown_:
		stream << "uknown";
		break;
	}
	stream << ",size=" << _size << ")" << std::endl;
	node_type::debug(stream, indent);
}

node* node_type_complex::on_child_added(node* n)
{
	const auto fields = dynamic_cast<node_type_complex_fields*>(n);
	if (fields != nullptr)
		_fields = fields;
	else
	{
		const auto methods = dynamic_cast<node_type_complex_methods*>(n);
		if (methods != nullptr)
			_methods = methods;
		else
		{
			const auto i = dynamic_cast<node_type_complex_inherits*>(n);
			if (i != nullptr)
				_inherits = i;
			else
			{
				const auto s = dynamic_cast<node_type_static_scope*>(n);
				if (s != nullptr)
					_static = s;
			}
		}
	}

	return node_type::on_child_added(n);
}

void node_type_complex::on_child_removed(node* n)
{
	node_type::on_child_removed(n);
	if (_fields == n)
		_fields = nullptr;
	else if (_methods == n)
		_methods = nullptr;
	else if (_inherits == n)
		_inherits = nullptr;
	else if (_static == n)
		_static = nullptr;
}

vector<node_type_complex_field*> node_type_complex::get_all_fields() const
{
	vector<node_type_complex_field*> result;
	get_all_fields(&result);
	return result;
}

void node_type_complex::get_all_fields(vector<node_type_complex_field*>* dest) const
{
	// if we already know that this is an interface or a trait then we also know that
	// this complex type doesn't have any fields
	if (_type == complex_type::interface_ || _type == complex_type::trait_)
	{
		return;
	}

	if (_inherits != nullptr)
	{
		for (const auto node: _inherits->get_children())
		{
			const auto inherited_from = dynamic_cast<const node_type_complex_inherit*>(node);
			if (inherited_from == nullptr)
				continue;
			const auto complex = dynamic_cast<const node_type_complex*>(inherited_from->get_inherits_from());
			if (complex == nullptr)
				continue;
			complex->get_all_fields(dest);
		}
	}

	if (_fields != nullptr)
	{
		for (const auto child: _fields->get_children())
		{
			const auto field = dynamic_cast<node_type_complex_field*>(child);
			// for now, no other children than field is allowed in the fields structure
			assert(field != nullptr);
			dest->add(field);
		}
	}
}


string node_type_complex::get_id() const
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

void node_type_complex::on_parent_node(node* p)
{
	superficial_collision_test(this);
}

void node_type_complex::write_json_properties(json& j)
{
	node_symbol::write_json_properties(j);
	j.write(json::pair{ "size", _size });
}

bool node_type_complex::superficial_test_symbol_collision(const node_type_complex* rhs) const
{
	if (get_name() == rhs->get_name())
		throw error_named_symbol_already_declared(get_source_code(), get_name(), rhs->get_source_code());
	return false;
}

void node_type_complex::on_process_phase(const recursion_detector* rd, resolve_state* state, int phase)
{
	if (phase != phase_resolve_size)
	{
		return;
	}

	// TODO: Figure out how large a struct's size should be. Might depend on if virtual or not.
	//       structs might also have no fields

	int size = 0;
	const recursion_detector rd0(rd, this);
	if (_inherits)
	{
		const auto children = _inherits->get_children();
		for (auto n: children)
		{
			const auto inherit = dynamic_cast<node_type_complex_inherit*>(n);
			if (inherit == nullptr)
				continue;
			inherit->process_phase(&rd0, state, phase);
			size += inherit->get_size();
		}
	}

	// A type might not have a size
	if (_fields == nullptr)
	{
		_size = size;
		return;
	}

	const auto fields = _fields->get_children();
	for (auto n: fields)
	{
		const auto field = dynamic_cast<node_type_complex_field*>(n);
		if (field == nullptr)
			continue;
		field->process_phase(&rd0, state, phase);
		size += field->get_size();
	}

	_size = size;
}

compatibility node_type_complex::is_compatible_with(node_type* rhs) const
{
	if (rhs == this)
		return compatibility::identical;
	return node_type::is_compatible_with(rhs);
}
