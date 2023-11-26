//
// Part of the o2 Project, under the Apache License v2.0 with o2 Project Exceptions.
// See the LICENSE file in the project root for license terms
//

#include "node_type_pointer_of.h"
#include "node_type_array.h"

using namespace o2;

string node_type_pointer_of::get_id() const
{
	stringstream ss;
	ss << "(*" << _pointer_of->get_id() << ")";
	return std::move(ss.str());
}

void node_type_pointer_of::debug(debug_ostream& stream, int indent) const
{
	stream << this << in(indent);
	stream << "type_pointer_of(node_type=" << _pointer_of << ')' << std::endl;
	node_type::debug(stream, indent);
}

compatibility node_type_pointer_of::is_compatible_with(node_type* rhs) const
{
	if (rhs == this)
		return compatibility::identical;

	const auto pointer_of = _pointer_of->get_type();

	const auto array = dynamic_cast<node_type_array*>(rhs);
	if (array)
	{
		// if the supplied type is an array type then the compiler can automatically cast the
		// value into a pointer if the array is of the same type as the pointer is of.
		const auto array_type = array->get_array_type()->get_type();
		if (array_type->is_compatible_with(pointer_of) == compatibility::identical)
			return compatibility::upcast;

		// otherwise you have to manually cast the type (for example you must explicitly cast [10]int into *void)
		// TODO: Take inheritance into consideration. For example [10]Class should not be cast to *BaseInterface.
		//       things like ptr++ does not work. You might have to do "&value[0]" to do this

	}

	const auto rhs_pointer_of = dynamic_cast<node_type_pointer_of*>(rhs);
	if (rhs_pointer_of)
	{
		const auto rhs_ptr = rhs_pointer_of->get_pointer_of_type()->get_type();

		// if the supplied type is a pointer of the same type
		if (rhs_ptr->is_compatible_with(pointer_of) == compatibility::identical)
			return compatibility::identical;

		// TODO: Inheritance?
	}

	return node_type::is_compatible_with(rhs);
}

node* node_type_pointer_of::on_child_added(node* n)
{
	const auto pointer_of = dynamic_cast<node_type*>(n);
	if (pointer_of)
		_pointer_of = pointer_of;
	return n;
}

void node_type_pointer_of::on_child_removed(node* n)
{
	if (_pointer_of == n)
		_pointer_of = nullptr;
}
