//
// Part of the o2 Project, under the Apache License v2.0 with o2 Project Exceptions.
// See the LICENSE file in the project root for license terms
//

#include "node_op_callfunc.h"
#include "../node_ref.h"
#include "../functions/node_func.h"

using namespace o2;

node_type* node_op_callfunc::get_type()
{
	return node_op::get_type();
}

node* node_op_callfunc::on_child_added(node* n)
{
	return node::on_child_added(n);
}

void node_op_callfunc::on_child_removed(node* n)
{
	node::on_child_removed(n);
}

void node_op_callfunc::debug(debug_ostream& stream, int indent) const
{
	stream << this << in(indent);
	stream << "op_callfunc()" << std::endl;
	node::debug(stream, indent);
}

bool node_op_callfunc::resolve(const recursion_detector* rd)
{
	if (_func != nullptr)
		return true;

	if (!node::resolve(rd))
		return false;

	auto ref = dynamic_cast<node_ref*>(get_child(0));
	if (ref == nullptr)
		throw expected_child_node(get_source_code(), "node_ref");

	// find which reference that fits best. The first child is a node_ref that's used to search for the function itself
	const auto num_args = get_child_count() - 1;
	for (auto potential_result: ref->get_result())
	{
		const auto func = dynamic_cast<node_func*>(potential_result);
		if (!func)
			continue;

		if (func->get_arguments() == nullptr || func->get_arguments()->get_child_count() == 0)
		{
			if (num_args == 0)
			{
				_func = func;
				break;
			}
			else
				continue;
		}

		// TODO: add support for varargs? or dynamic number of template arguments
		const auto args = func->get_arguments()->get_children();
		if (args.size() != num_args)
			continue;

		// verify each argument
		const int num_children = args.size();
		int i = 0;
		for (; i < num_children; ++i)
		{
			const auto named_arg = static_cast<node_var*>(args[i]);
			const auto named_arg_type = named_arg->get_type();

			// the first child of this type is the reference to the function we want to call
			const auto arg_op = static_cast<node_op*>(get_child(i + 1));
			const auto arg_type = arg_op->get_type();

			if (named_arg_type != arg_type)
				break;
		}

		if (i == num_children)
		{
			_func = func;
			break;
		}
	}

	// Could not resolve the function
	if (_func == nullptr)
		throw resolve_error_unresolved_reference(get_source_code());

	return true;
}
