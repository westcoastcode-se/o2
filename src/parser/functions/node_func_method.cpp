//
// Part of the o2 Project, under the Apache License v2.0 with o2 Project Exceptions.
// See the LICENSE file in the project root for license terms
//

#include "node_func_method.h"

using namespace o2;

void node_func_method::debug(debug_ostream& stream, int indent) const
{
	stream << this << in(indent);
	stream << "func_method(name=" << get_name() << ",body=" << get_body() << ")" << std::endl;
	node::debug(stream, indent);
}

string node_func_method::get_id() const
{
	stringstream ss;
	const auto symbol = get_parent_of_type<node_symbol>();
	if (symbol)
		ss << symbol->get_id();
	ss << '/';
	ss << get_name();
	ss << "(this";
	if (get_parameters() != nullptr)
	{
		const auto children = get_parameters()->get_parameters();
		for (auto i : children)
		{
			const auto type = i->get_type();
			ss << ',';
			ss << type->get_id();
		}
	}
	ss << ')';
	ss << '(';
	if (get_returns() != nullptr)
	{
		const auto children = get_returns()->get_children();
		for (int i = 0; i < children.size(); ++i)
		{
			const auto type = dynamic_cast<node_type*>(children[i]);
			if (i > 0)
				ss << ',';
			ss << type->get_id();
		}
	}
	ss << ')';
	return std::move(ss.str());
}
