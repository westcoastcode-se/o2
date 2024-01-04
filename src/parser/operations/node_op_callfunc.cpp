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

namespace
{
	struct func_lookup
	{
		// the function
		node_func* func;
		// how many arguments are identical
		int num_identical;
	};
}

void node_op_callfunc::resolve0(const recursion_detector* rd, resolve_state* state)
{
	if (_func != nullptr)
		return;

	node::resolve0(rd, state);

	// assume that the first child is the node reference
	auto ref = dynamic_cast<node_ref*>(get_child(0));
	if (ref == nullptr)
		throw expected_child_node(get_source_code(), "node_ref");

	// find which reference that fits best. The first child is a node_ref that's used to search for the function itself
	std::vector<func_lookup> potential_funcs;
	const auto num_args = get_child_count() - 1;
	for (auto potential_result: ref->get_result())
	{
		const auto func = dynamic_cast<node_func*>(potential_result);
		if (!func)
			continue;

		if (func->get_parameters() == nullptr || func->get_parameters()->get_child_count() == 0)
		{
			// are all arguments identical, then this is the best function. If multiple functions have identical
			// arguments the select the first one (i.e. the one who is imported last in the source code).
			//
			// If developer want to select another then they have to set an alias for the import
			if (num_args == 0)
			{
				_func = func;
				return;
			}
			// TODO: consider optional arguments
			continue;
		}

		// TODO: add support for varargs
		// TODO: dynamic number of template arguments
		// TODO: consider optional arguments
		const auto args = func->get_parameters()->get_children();
		if (args.size() != num_args)
			continue;

		// verify each argument
		// TODO: Add support for figuring out the best function among many functions for
		//       upcast among primitives and inheritance
		const int num_children = args.size();
		int i = 0;
		int num_identical = 0;
		for (; i < num_children; ++i)
		{
			const auto named_arg = static_cast<node_var*>(args[i]);
			const auto named_arg_type = named_arg->get_type();

			// index 0 is the reference to the function we want to call. All the other children
			// are the actual values we send into the function
			const auto arg_op = static_cast<node_op*>(get_child(i + 1));
			const auto arg_type = arg_op->get_type();

			const recursion_detector rd0(rd, this);
			named_arg_type->process_phase(&rd0, state, phase_resolve);
			arg_type->process_phase(&rd0, state, phase_resolve);

			// is the compatibility not identical or upcast?
			const auto c = named_arg_type->get_type()->is_compatible_with(arg_type->get_type());
			if (c == compatibility::identical)
				num_identical++;
			if ((int)c > (int)compatibility::upcast)
				break;
		}

		// if we've tested all arguments and all of them are identical or upcast compatible then
		// this is a function that we can call
		if (i == num_children)
		{
			// are all arguments identical, then this is the best function. If multiple functions have identical
			// arguments the select the first one (i.e. the one who is imported last in the source code).
			//
			// If developer want to select another then they have to set an alias for the import
			if (num_identical == num_children)
			{
				_func = func;
				return;
			}
			potential_funcs.push_back({ func, num_children });
		}
	}

	if (potential_funcs.empty())
		throw resolve_error_unresolved_reference(get_source_code());
	else if (potential_funcs.size() == 1)
		_func = potential_funcs[0].func;
	else if (potential_funcs.size() > 1)
	{
		// select the most compatible function.
		//
		// things to take into consideration:
		// - how many of the arguments are of the identical type
		// - are we supplying a pointer, then select the function with closest type based on the inheritance tree
		// - if it's a pointer to a memory location then allow for "*void"

		node_func* func = nullptr;
		int num_identical = 0;
		int identical_count = 0;
		for (int i = 0; i < potential_funcs.size(); ++i)
		{
			int pni = potential_funcs[i].num_identical;
			if (pni > num_identical)
			{
				func = potential_funcs[i].func;
				num_identical = pni;
				identical_count = 1;
			}
			else if (pni == num_identical)
			{
				identical_count++;
			}
		}

		// since multiple functions have the same number of "upcast" types, the compiler can know for sure
		// which one the developer want to select.
		//
		// developer has to cast one or more arguments in order to specify which function to use
		if (identical_count > 1)
			throw resolve_error_multiple_refs(get_source_code());

		_func = func;
	}
}
