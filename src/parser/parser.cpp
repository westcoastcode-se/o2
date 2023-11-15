//
// Part of the o2 Project, under the Apache License v2.0 with o2 Project Exceptions.
// See the LICENSE file in the project root for license terms
//

#include "parser.h"
#include "parser_scope.h"
#include "token.h"
#include "types/node_type_array.h"
#include "types/node_type_ref.h"
#include "functions/node_func_arguments.h"
#include "functions/node_func_returns.h"
#include "operations/node_op_return.h"
#include "module/module.h"
#include "types/node_type_implicit.h"
#include "operations/node_op_assign.h"
#include "node_link.h"
#include "variables/node_var_const.h"
#include "operations/node_op_callfunc.h"

using namespace o2;

namespace
{
	node_func_method* parse_func_method(const parser_scope* ps);

	node_func* parse_func_extern(const parser_scope* ps);

	node_func* parse_func(const parser_scope* ps, int modifier);

	node_type* parse_type(const parser_scope* ps);

	node_ref* parse_ref(const parser_scope* ps, int query, int chain_types, int query_flags);

	node_type* resolve_type_from_pmv(const parser_scope* ps, const primitive_value& pmv)
	{
		const auto type = ps->state->get_primitive_type(pmv.type);
		return o2_new node_type_ref(ps->get_view(), type);
	}

	node_op_constant* parse_op_constant(const parser_scope* ps)
	{
		const auto t = ps->t;

		primitive_value pmv;
		switch (t->type())
		{
		case token_type::number:
			if (t->is_modifier(token_modifier::hint_unsigned))
			{
				const auto value = t->value_uint64();
				if (value > UINT32_MAX)
				{
					pmv.type = primitive_type::uint64;
					pmv.u64 = value;
				}
				else
				{
					pmv.type = primitive_type::uint32;
					pmv.u32 = static_cast<std::uint32_t>(value);
				}
			}
			else
			{
				const auto value = t->value_int64();
				if (value > INT32_MAX)
				{
					pmv.type = primitive_type::int64;
					pmv.i64 = value;
				}
				else
				{
					pmv.type = primitive_type::int32;
					pmv.i32 = static_cast<std::int32_t>(value);
				}
			}
			break;
		case token_type::hex:
			pmv.u64 = t->value_uint64();
			pmv.type = primitive_type::uint64;
			if (pmv.u64 <= UINT32_MAX)
			{
				pmv.u32 = static_cast<std::uint32_t>(pmv.u64);
				pmv.type = primitive_type::uint32;
			}
			break;
		case token_type::boolean:
			pmv.type = primitive_type::bool_;
			pmv.bool_ = t->value_bool() ? 1 : 0;
			break;
		case token_type::decimal:
			if (t->is_modifier(token_modifier::hint_float))
			{
				pmv.type = primitive_type::float32;
				pmv.f32 = t->value_float32();
			}
			else
			{
				pmv.type = primitive_type::float64;
				pmv.f64 = t->value_float64();
			}
			break;
		case token_type::string:
			throw error_not_implemented(ps->get_view(), t);
		default:
			throw error_expected_constant(ps->get_view(), t);
		}

		auto result = o2_new node_op_constant(ps->get_view(), pmv);
		auto guard = memory_guard(result);
		result->add_child(resolve_type_from_pmv(ps, pmv));
		t->next();
		return guard.done();
	}

	/**
	 * \brief 
	 */
	typedef node_op* (* parse_op_fn)(const parser_scope* ps);

	node_op* parse_op_factor(const parser_scope* ps);

	node_op* parse_op_compare(const parser_scope* ps);

	node_op* parse_op_assign(const parser_scope* ps);

	node_op* parse_op_unary(const parser_scope* ps, token_type tt, parse_op_fn right_fn);

	node_op* parse_op_binop(const parser_scope* ps, array_view<token_type> tokens,
			parse_op_fn left_fn, parse_op_fn right_fn);

	node_op* parse_op_atom(const parser_scope* ps)
	{
		const auto t = ps->t;
		switch (t->type())
		{
		case token_type::number:
		case token_type::decimal:
		case token_type::boolean:
		case token_type::hex:
		case token_type::string:
			return parse_op_constant(ps);
		case token_type::parant_left:
			t->next();
			{
				const auto node = parse_op_assign(ps);
				auto guard = memory_guard(node);
				if (t->type() != token_type::parant_right)
					throw error_syntax_error(ps->get_view(), t, "expected ')'");
				t->next();
				return guard.done();
			}
		case token_type::identity:
		{
			// todo: add support for variables
			const auto ref = parse_ref(ps, node_ref::only_callable,
					node_ref::only_callable_chain_types,
					node::query_flag_parents |
					node::query_flag_follow_refs |
					node::query_flag_children_from_root);
			auto guard = memory_guard(ref);

			// is the token after the identity a '(' then assume that we are making a function call
			if (t->type() == token_type::parant_left)
			{
				const auto call = o2_new node_op_callfunc(ref->get_source_code());
				auto guard0 = memory_guard(call);
				call->add_child(guard.done());

				t->next_until_not(token_type::comment);
				if (t->type() != token_type::parant_right)
				{
					const parser_scope ps0(ps, call);
					while (true)
					{
						call->add_child(parse_op_assign(&ps0));

						switch (t->type())
						{
						case token_type::comma:
							t->next_until_not(token_type::comment);
							continue;
						case token_type::parant_right:
							goto done;
						default:
							throw error_syntax_error(ps->get_view(), t, "expected ')' or ','");
						}
					}
				done:;
				}
				t->next();
				return guard0.done();
			}
			else if (t->type() == token_type::test_lt)
			{
				throw error_not_implemented(ps->get_view(),
						"macro style function calls or type references are not supported yet");
			}
			else
			{
				throw error_not_implemented(ps->get_view(),
						"variables not supported yet");
			}
		}
		default:
			throw error_not_implemented(ps->get_view(), t);
		}
	}

	node_op* parse_op_factor(const parser_scope* ps)
	{
		const auto t = ps->t;
		switch (t->type())
		{
		case token_type::minus:
		case token_type::plus:
		case token_type::bit_not:
		case token_type::dec:
		case token_type::inc:
			return parse_op_unary(ps, t->type(), parse_op_factor);
		default:
			return parse_op_atom(ps);
		}
	}

	node_op* parse_op_term(const parser_scope* ps)
	{
		static const vector<token_type> PARSE_TOKENS(
				token_type::pointer, token_type::div
		);
		return parse_op_binop(ps, PARSE_TOKENS, parse_op_factor, parse_op_factor);
	}

	node_op* parse_op_expression(const parser_scope* ps)
	{
		static const vector<token_type> PARSE_TOKENS(
				token_type::plus, token_type::minus
		);
		return parse_op_binop(ps, PARSE_TOKENS, parse_op_term, parse_op_term);
	}

	node_op* parse_op_unary(const parser_scope* ps, token_type tt, parse_op_fn right_fn)
	{
		const auto view = ps->get_view();
		ps->t->next();

		const auto right = right_fn(ps);
		auto guard = memory_guard(right);

		const auto unary = o2_new node_op_unaryop(view, node_op_unaryop::from_token_type(tt));
		auto guard2 = memory_guard(unary);
		unary->add_child(right);
		guard.done();
		return guard2.done();
	}

	node_op* parse_op_binop(const parser_scope* ps, array_view<token_type> tokens,
			parse_op_fn left_fn, parse_op_fn right_fn)
	{
		const auto t = ps->t;
		if (right_fn == nullptr)
			right_fn = left_fn;

		const auto left = left_fn(ps);
		auto guard = memory_guard(left);

		const auto size = tokens.size();
		for (int i = 0; i < size; ++i)
		{
			const auto view = ps->get_view();
			const auto tt = t->type();
			if (tokens[i] != tt)
				continue;
			t->next();

			const auto right = right_fn(ps);
			auto guard1 = memory_guard(right);
			const auto new_left = o2_new node_op_binop(view, node_op_binop::from_token_type(tt));
			auto guard2 = memory_guard(new_left);
			new_left->add_child(left);
			guard.done();
			new_left->add_child(right);
			guard1.done();
			guard = memory_guard(guard2.done());

			// retry parsing. -1 will become 0 after "++i" is done in the for loop
			i = -1;
		}
		return guard.done();
	}

	node_op* parse_op_compare(const parser_scope* ps)
	{
		const auto t = ps->t;
		if (t->type() == token_type::not_)
			return parse_op_unary(ps, t->type(), parse_op_factor);

		static const vector<token_type> PARSE_COMPARE_TOKENS(
				token_type::test_equals, token_type::not_,
				token_type::test_lt, token_type::test_lte,
				token_type::test_gt, token_type::test_gte
		);

		return parse_op_binop(ps, PARSE_COMPARE_TOKENS, parse_op_expression, parse_op_expression);
	}

	node_op* parse_op_assign(const parser_scope* ps)
	{
		const auto t = ps->t;
		if (t->type() == token_type::assign)
		{
			const auto assign = o2_new node_op_assign(ps->get_view());
			auto guard = memory_guard(assign);
			t->next_until_not(token_type::comment);
			const parser_scope ps0(ps, assign);
			assign->add_child(parse_op_assign(&ps0));
			return guard.done();
		}
		return parse_op_compare(ps);
	}

	node* parse_op(const parser_scope* ps)
	{
		const auto t = ps->t;
		if (token::is_keyword(t->type()))
		{
			if (t->type() == token_type::func)
			{
				return parse_func(ps, 0);
			}
			else if (t->type() == token_type::type)
			{
				return parse_type(ps);
			}
			else if (t->type() == token_type::return_)
			{
				const auto ret = o2_new node_op_return(ps->get_view());
				auto guard = memory_guard(ret);
				t->next();
				const parser_scope ps0(ps, ret);
				ret->add_child(parse_op_compare(&ps0));
				if (t->type() != token_type::newline)
					throw error_syntax_error(ps->get_view(), t, "expected newline");
				return guard.done();
			}
			throw error_not_implemented(ps->get_view(), t);
		}
		else if (t->type() == token_type::bracket_left)
		{
			const auto scope = o2_new node_scope(ps->get_view());
			auto guard = memory_guard(scope);

			while (t->next_until_not(token_type::newline, token_type::comment) != token_type::bracket_right)
			{
				const parser_scope ps0(ps, scope);
				scope->add_child(parse_op(&ps0));
				if (t->type() != token_type::bracket_right)
					throw error_syntax_error(ps->get_view(), t, "expected '}'");
			}
			return guard.done();
		}
		else if (t->type() == token_type::eof)
		{
			throw error_syntax_error(ps->get_view(), t, "expected '}'");
		}

		return parse_op_assign(ps);
	}

	/**
	 * \param ps
	 * \param query what are the types we are looking
	 * \param chain_types if this is a chain of references, what are the types that allows for the final
	 * 					  reference to be found
	 * \param query_flags how the query is performed during the resolve phase
	 * \return a reference node to be added in the tree
	 */
	node_ref* parse_ref(const parser_scope* ps, int query, int chain_types, int query_flags)
	{
		// allow for
		// <identity><macro>[.<identity>]
		// macros are only allowed at the last part of the reference chain

		const auto t = ps->t;
		if (t->type() != token_type::identity)
			throw error_expected_identity(ps->get_view(), t);

		const auto ref = o2_new node_ref(ps->get_view(), chain_types, query_flags, t->value());
		auto guard = memory_guard(ref);
		auto section = ref;

		// parse for reference chain sections
		while (t->next() == token_type::dot)
		{
			if (t->next() != token_type::identity)
				throw error_expected_identity(ps->get_view(), t);

			const auto new_section = o2_new node_ref(ps->get_view(), chain_types,
					node_ref::query_flag_children, t->value());
			auto inner_guard = memory_guard(new_section);
			section->add_child(new_section);
			section = inner_guard.done();
		}
		section->set_query_types(query);
		return guard.done();
	}

	/**
	 * \brief parse the content of the array accessor
	 * \param ps
	 * \return
	 */
	node_type_array* parse_arg_type_array(const parser_scope* ps)
	{
		const auto t = ps->t;
		if (t->type() != token_type::square_left)
			throw error_syntax_error(ps->get_view(), ps->t, "expected '['");

		auto arr = memory_guard(o2_new node_type_array(ps->get_view()));
		t->next();
		const auto op = parse_op_assign(ps);
		arr->add_child(op);

		if (ps->t->type() != token_type::square_right)
			throw error_syntax_error(ps->get_view(), ps->t, "expected ']'");
		t->next();
		return arr.done();
	}

	node_type_accessor* parse_arg_type_pointer(const parser_scope* ps)
	{
		const auto t = ps->t;
		if (t->type() != token_type::pointer)
			throw error_syntax_error(ps->get_view(), ps->t, "expected '*'");
		auto acc = memory_guard(o2_new node_type_accessor(ps->get_view(), node_type_accessor::accessor_pointer));
		t->next();
		return acc.done();
	}

	node_type_accessor* parse_arg_type_reference(const parser_scope* ps)
	{
		const auto t = ps->t;
		if (t->type() != token_type::ref)
			throw error_syntax_error(ps->get_view(), ps->t, "expected '&'");
		auto acc = memory_guard(o2_new node_type_accessor(ps->get_view(), node_type_accessor::accessor_reference));
		t->next();
		return acc.done();
	}

	/**
	 * \brief parse a type
	 * \param ps
	 * \return the node that represents the type
	 */
	node_type* parse_type_ref(const parser_scope* ps)
	{
		const auto t = ps->t;
		const auto tt = t->type();

		// if array, then query until parse until
		if (tt == token_type::square_left)
		{
			auto guard = memory_guard(parse_arg_type_array(ps));
			const parser_scope ps0(ps, guard.get());
			guard->add_child(parse_type_ref(&ps0));
			return guard.done();
		}

		// if pointer *
		if (tt == token_type::pointer)
		{
			auto guard = memory_guard(parse_arg_type_pointer(ps));
			const parser_scope ps0(ps, guard.get());
			guard->add_child(parse_type_ref(&ps0));
			return guard.done();
		}

		// if reference &
		if (tt == token_type::ref)
		{
			auto guard = memory_guard(parse_arg_type_reference(ps));
			const parser_scope ps0(ps, guard.get());
			guard->add_child(parse_type_ref(&ps0));
			return guard.done();
		}

		auto type_ref = o2_new node_type_ref(ps->get_view());
		auto guard = memory_guard(type_ref);
		const parser_scope ps0(ps, type_ref);
		type_ref->add_child(parse_ref(&ps0,
				node_ref::only_types,
				node_ref::only_type_chain_types,
				node::query_flag_parents |
				node::query_flag_follow_refs |
				node::query_flag_children_from_root));
		ps->state->add_resolve_size(type_ref);
		return guard.done();
	}

	node_type* parse_arg_type(const parser_scope* ps)
	{
		return parse_type_ref(ps);
	}

	node_var* parse_func_arg(const parser_scope* ps)
	{
		const auto t = ps->t;
		if (t->type() != token_type::identity)
			throw error_expected_identity(ps->get_view(), t);

		const auto var = o2_new node_var(ps->get_view(), t->value(), 0);
		auto guard = memory_guard(var);
		t->next();
		var->add_child(parse_arg_type(ps));
		return guard.done();
	}

	void parse_func_returns(const parser_scope* ps, node_func* func)
	{
		const auto returns = o2_new node_func_returns(ps->get_view());
		func->add_child(returns);

		const auto t = ps->t;
		if (t->type() == token_type::comment)
			t->next_until_not(token_type::comment);

		if (t->type() != token_type::bracket_left && t->type() != token_type::newline)
		{
			// ignore void keyword
			if (t->type() == token_type::void_)
			{
				t->next();
				return;
			}

			const parser_scope ps0(ps, returns);
			returns->add_child(parse_type_ref(&ps0));
		}
	}

	node_func* parse_func0(const parser_scope* ps, int modifiers)
	{
		const auto t = ps->t;
		auto tt = t->next();
		if (tt != token_type::identity)
			throw error_expected_identity(ps->get_view(), t);

		node_func* func;
		if (ps->func != nullptr)
			func = o2_new node_func(ps->get_view(), t->value(), node_func::inner_function{});
		else if (bit_isset(modifiers, node_func::modifier_const))
			func = o2_new node_func(ps->get_view(), t->value(), node_func::const_function{});
		else if (bit_isset(modifiers, node_func::modifier_extern))
			func = o2_new node_func(ps->get_view(), t->value(), node_func::extern_function{});
		else
			func = o2_new node_func(ps->get_view(), t->value());
		auto guard = memory_guard(func);

		tt = t->next();
		if (tt == token_type::test_gt)
			throw error_not_implemented(ps->get_view(), "macros not supported yet");
		if (tt != token_type::parant_left)
			throw error_syntax_error(ps->get_view(), t, "expected '('");

		// parse arguments:
		// syntax: name type[, name type, ...]
		const auto arguments = o2_new node_func_arguments(ps->get_view());
		func->add_child(arguments);
		tt = t->next_until_not(token_type::comment);
		if (tt != token_type::parant_right)
		{
			const parser_scope ps0(ps, arguments);
			while (true)
			{
				if (t->type() == token_type::void_)
				{
					if (t->next_until_not(token_type::comment) != token_type::parant_right)
					{
						throw error_syntax_error(ps->get_view(), t, "expected ')' after void keyword");
					}
					break;
				}

				arguments->add_child(parse_func_arg(&ps0));
				switch (t->type())
				{
				case token_type::comma:
					t->next_until_not(token_type::comment);
					continue;
				case token_type::parant_right:
					goto done;
				default:
					throw error_syntax_error(ps->get_view(), t, "expected ')' or ','");
				}
			}
		done:;
		}
		t->next();

		parse_func_returns(ps, func);
		return guard.done();
	}

	node_func_body* parse_func_body(const parser_scope* ps)
	{
		const auto t = ps->t;
		if (t->type() != token_type::bracket_left)
			throw error_syntax_error(ps->get_view(), t, "expected '{'");

		// Create a new body node
		const auto body = o2_new node_func_body(ps->get_view());
		auto guard = memory_guard(body);

		// Add the initial scope for the function body
		const auto scope = o2_new node_scope(ps->get_view());
		auto guard2 = memory_guard(scope);
		body->add_child(scope);
		guard2.done();

		// Read function body nodes
		const parser_scope ps0(ps, scope);
		while (t->next_until_not(token_type::newline, token_type::comment) != token_type::bracket_right)
			scope->add_child(parse_op(&ps0));
		return guard.done();
	}

	node_func* parse_func_extern(const parser_scope* ps)
	{
		const auto func = parse_func0(ps, node_func::modifier_extern);
		auto guard = memory_guard(func);
		// TODO should it be possible to specify a function, but then
		//      allow it to be overridden by external library?
		const auto t = ps->t;
		if (t->type() == token_type::comment)
			t->next_until_not(token_type::comment);
		if (t->type() != token_type::newline && t->type() != token_type::eof)
			throw error_unexpected_extern_func_body(ps->get_view());
		return guard.done();
	}

	node_func_method* parse_func_method0(const parser_scope* ps)
	{
		const auto t = ps->t;
		auto tt = t->next();
		if (tt != token_type::identity)
			throw error_expected_identity(ps->get_view(), t);

		const auto func = o2_new node_func_method(ps->get_view(), t->value());
		auto guard = memory_guard(func);

		tt = t->next();
		if (tt == token_type::test_gt)
			throw error_not_implemented(ps->get_view(), "macros not supported yet");
		if (tt != token_type::parant_left)
			throw error_syntax_error(ps->get_view(), t, "expected '('");

		// parse arguments:
		// syntax: name type[, name type, ...]
		const auto arguments = o2_new node_func_arguments(ps->get_view());
		func->add_child(arguments);
		tt = t->next();

		if (tt == token_type::parant_right)
		{
			// automatically add the this argument
			const auto this_var = o2_new node_var_this(ps->get_view(), STR("this"), ps->type);
			arguments->add_child(this_var);
		}
		else
		{
			// first argument might be a this argument
			if (t->type() == token_type::this_)
			{
				const auto this_var = o2_new node_var_this(ps->get_view(), STR("this"), ps->type);
				arguments->add_child(this_var);
				t->next();
			}
			else
			{
				// It might still be a this argument if the "type" is this
				// TODO: Add support for named this arguments
				const auto this_var = o2_new node_var_this(ps->get_view(), STR("this"), ps->type);
				arguments->add_child(this_var);
			}

			const parser_scope ps0(ps, arguments);
			while (t->type() != token_type::parant_right)
			{
				if (t->type() == token_type::void_)
				{
					if (t->next_until_not(token_type::comment) != token_type::parant_right)
					{
						throw error_syntax_error(ps->get_view(), t, "expected ')' after void keyword");
					}
					continue;
				}

				arguments->add_child(parse_func_arg(&ps0));
				switch (t->type())
				{
				case token_type::comma:
					t->next_until_not(token_type::comment);
					continue;
				case token_type::parant_right:
					continue;
				default:
					throw error_syntax_error(ps->get_view(), t, "expected ')' or ','");
				}
			}
		done:;
		}
		t->next();

		parse_func_returns(ps, func);
		return guard.done();
	}

	node_func_method* parse_func_method(const parser_scope* ps)
	{
		const auto func = parse_func_method0(ps);
		auto guard = memory_guard(func);

		// this function definition have a body
		const auto t = ps->t;
		if (t->type() == token_type::bracket_left)
		{
			const parser_scope ps0(ps, func);
			func->add_child(parse_func_body(&ps0));
		}

		return guard.done();
	}

	node_func* parse_func(const parser_scope* ps, int modifiers)
	{
		const auto func = parse_func0(ps, modifiers);
		auto guard = memory_guard(func);

		// this function definition have a body
		const auto t = ps->t;
		if (t->type() == token_type::bracket_left)
		{
			const parser_scope ps0(ps, func);
			func->add_child(parse_func_body(&ps0));
		}

		return guard.done();
	}

	node* parse_named_constant(const parser_scope* ps)
	{
		// constants must have one of the following syntax
		// const <name> = <expr>
		// const <name> <type> = <expr>

		const auto t = ps->t;
		if (t->type() != token_type::identity)
			throw error_expected_identity(ps->get_view(), t);

		const auto var = o2_new node_var_const(ps->get_view(), t->value());
		auto guard = memory_guard(var);
		const parser_scope ps1(ps, var);
		if (t->next() == token_type::identity)
		{
			// const <name> <type> = <expr>
			var->add_child(parse_arg_type(&ps1));
			if (t->type() != token_type::assign)
				throw error_syntax_error(ps1.get_view(), t, "expected '='");
			t->next_until_not(token_type::comment);
			var->add_child(parse_op_compare(&ps1));
		}
		else
		{
			// const <name> = <expr>
			const auto implicit = o2_new node_type_implicit(ps1.get_view());
			var->add_child(implicit);

			if (t->type() != token_type::assign)
				throw error_syntax_error(ps1.get_view(), t, "expected '='");
			t->next_until_not(token_type::comment);

			// create a link that can be used between the implicit_type and the expression
			const auto link = o2_new node_link(ps->get_view());
			implicit->add_child(link);
			const auto op = parse_op_compare(&ps1);
			op->add_child(link->new_link());
			var->add_child(op);
			ps->state->add_resolve_size(implicit);
		}

		return guard.done();
	}

	node* parse_extern(const parser_scope* ps)
	{
		const auto t = ps->t;
		t->next();

		// Only things allowed to be extern are:
		// functions

		switch (t->type())
		{
		case token_type::func:
			return parse_func_extern(ps);
		default:
			throw error_syntax_error(ps->get_view(), t,
					"only functions are allowed to be external");
		}
	}

	node* parse_const(const parser_scope* ps)
	{
		const auto t = ps->t;
		t->next();

		// Only things allowed to be const are:
		// variables (identity)
		// functions
		// types

		switch (t->type())
		{
		case token_type::func:
			return parse_func(ps, node_func::modifier_const);
		case token_type::identity:
			return parse_named_constant(ps);
		default:
			throw error_syntax_error(ps->get_view(), t,
					"only functions, variables and types are allowed to be constant");
		}
	}

	node_type_struct_field* parse_type_field(const parser_scope* ps)
	{
		const auto t = ps->t;
		if (t->type() != token_type::identity)
			throw error_expected_identity(ps->get_view(), t);

		const auto field = o2_new node_type_struct_field(ps->get_view(), t->value());
		auto guard = memory_guard(field);
		t->next();
		field->add_child(parse_type_ref(ps));
		if (t->type() != token_type::newline)
			throw error_syntax_error(ps->get_view(), t, "expected newline at the end of a field");
		return guard.done();
	}

	void parse_type_statics(const parser_scope* ps, node_type_struct_static* static_)
	{
		const auto t = ps->t;
		if (t->next_until_not(token_type::comment) != token_type::bracket_left)
			throw error_syntax_error(ps->get_view(), t, "expected '{'");

		// parse until we've reached the end of the static block
		while (t->next_until_not(token_type::newline, token_type::comment) != token_type::bracket_right)
		{
			switch (t->type())
			{
			case token_type::var:
			{
				if (static_->get_vars() == nullptr)
				{
					const auto vars = o2_new node_type_struct_static_vars(static_->get_source_code());
					static_->add_child(vars);
				}
				t->next_until_not(token_type::comment);
				const parser_scope ps1(ps, static_->get_vars());
				static_->get_vars()->add_child(parse_type_field(&ps1));
				continue;
			}
			case token_type::func:
			{
				if (static_->get_funcs() == nullptr)
				{
					const auto funcs = o2_new node_type_struct_static_funcs(static_->get_source_code());
					static_->add_child(funcs);
				}
				const parser_scope ps1(ps, static_->get_funcs());
				static_->get_funcs()->add_child(parse_func(&ps1, 0));
				continue;
			}
			case token_type::bracket_right:
				break;
			default:
				throw error_syntax_error(ps->get_view(), t, "expected 'var' or 'func'");
			}
		}

		if (t->type() != token_type::bracket_right)
			throw error_syntax_error(ps->get_view(), t, "expected '}'");
	}

	node_type* parse_type(const parser_scope* ps)
	{
		// types can have the format:
		//
		// type <name> : <parent>\n
		// type <name> : <parent> {}
		// type <name> : <parent> {\n}
		// type <name>\n
		// type <name> {}
		// type <name> {\n}

		const auto t = ps->t;
		if (t->next_until_not(token_type::comment) != token_type::identity)
			throw error_expected_identity(ps->get_view(), t);

		const auto identity = t->value();
		const auto type = o2_new node_type_struct(ps->get_view(), identity);
		ps->state->add_resolve_size(type);
		auto guard = memory_guard(type);
		const parser_scope ps0(ps, type);

		// Next token
		t->next();

		// If < token then this is a macro type
		if (t->type() == token_type::test_lt)
		{
			throw error_not_implemented(ps->get_view(), "marco types are not implemented yet");
		}

		// type <name> [: <inherited from>] { ... }
		if (t->type() == token_type::colon)
		{
			// Only inheriting from structs are allowed for now
			if (t->next() != token_type::struct_)
				throw error_not_implemented(ps->get_view(), "inheritance are not implemented yet");
			t->next();
		}

		if (t->type() == token_type::newline)
		{
			// do nothing
		}
		else if (t->type() == token_type::bracket_left)
		{
			// types can have the following keywords:
			// var - variables
			// func - methods
			// static - static scope for static variables and functions

			node_type_struct_fields* fields = nullptr;
			node_type_struct_methods* methods = nullptr;
			node_type_struct_static* static_ = nullptr;

			// Seek the first token
			while (t->next_until_not(token_type::newline, token_type::comment) != token_type::bracket_right)
			{
				switch (t->type())
				{
				case token_type::var:
				{
					if (fields == nullptr)
					{
						fields = o2_new node_type_struct_fields(type->get_source_code());
						type->add_child(fields);
					}
					t->next_until_not(token_type::comment);
					const parser_scope ps1(&ps0, fields);
					fields->add_child(parse_type_field(&ps1));
					continue;
				}
				case token_type::type:
				{
					type->add_child(parse_type(&ps0));
					continue;
				}
				case token_type::func:
				{
					if (methods == nullptr)
					{
						methods = o2_new node_type_struct_methods(type->get_source_code());
						type->add_child(methods);
					}
					const parser_scope ps1(&ps0, methods);
					methods->add_child(parse_func_method(&ps1));
					continue;
				}
				case token_type::static_:
				{
					if (static_ == nullptr)
					{
						static_ = o2_new node_type_struct_static(type->get_source_code());
						type->add_child(static_);
					}

					const parser_scope ps1(&ps0, static_);
					parse_type_statics(&ps1, static_);
					continue;
				}
				default:
					throw error_syntax_error(ps->get_view(), t, "expected 'var', 'type', 'func' or 'static'");
				}
			}
			t->next();
		}
		else if (t->type() != token_type::eof)
			throw error_syntax_error(ps->get_view(), t, "expected newline or '{' at the start of type definition");
		return guard.done();
	}

	node_import* parse_import(const parser_scope* ps)
	{
		const auto t = ps->t;
		if (t->next() != token_type::string)
			throw error_expected_constant(ps->get_view(), t);

		const auto view = ps->get_view();
		const auto import_statement = t->value();
		t->next();
		string_view alias;
		if (t->type() == token_type::as)
		{
			if (t->next() != token_type::identity)
				throw error_expected_identity(ps->get_view(), t);
			alias = t->value();
			t->next();
		}
		auto import = o2_new node_import(view, import_statement, alias);
		auto guard = memory_guard(import);
		return guard.done();
	}

	/**
	 * @brief parse the package scope
	 * @param ps the current state
	 */
	void parse_package_scope(const parser_scope* ps)
	{
		const auto t = ps->t;
		while (true)
		{
			switch (t->type())
			{
			case token_type::import:
				throw error_syntax_error(ps->get_view(), t,
						"imports are only allowed at the top of the package segment");
			case token_type::func:
				ps->closest->add_child(parse_func(ps, 0));
				continue;
			case token_type::const_:
				ps->closest->add_child(parse_const(ps));
				continue;
			case token_type::extern_:
				ps->closest->add_child(parse_extern(ps));
				continue;
			case token_type::type:
				ps->closest->add_child(parse_type(ps));
				continue;
			case token_type::eof:
				return;
			default:
				t->next();
				break;
			}
		}
	}

	/**
	 * @brief parse tokens inside a package scope
	 * @param ps the current state
	 */
	void parse_package_pre_scope(const parser_scope* ps)
	{
		const auto t = ps->t;

		// Parse import tokens. Imports are only allowed to be at the top of the package
		node* parent = ps->closest;
		while (true)
		{
			switch (t->type())
			{
			case token_type::import: {
					const parser_scope ps0(ps, parent);
					auto import = parse_import(&ps0);
					ps->state->add_import(import);
					parent->add_child(import);
					parent = import;
				}
			case token_type::comment:
			case token_type::newline:
				t->next();
				break;
			default:
				goto parse_package_pre_scope_done;
			}
		}
	parse_package_pre_scope_done:
		const parser_scope ps1(ps, parent);
		parse_package_scope(&ps1);
	}
}

node_package* o2::parse_module_file(syntax_tree* st, const module* m, source_code src,
		parser_state* state)
{
	assert(m);
	assert(st);
	assert(state);

	// application entry point functionality is always put in the root package
	const auto app_package = st->get_root_package();

	// parse source code found
	lexer l(src.get_text());
	token t(&l);
	state->set_source_code(&src);

	collision_detector detector;
	const parser_scope ps0(state, &t);
	const parser_scope ps1(&ps0, app_package);
	t.next();
	parse_package_pre_scope(&ps1);
	return app_package;
}

node_package* o2::parse_module_path(syntax_tree* st, const module* m, string_view path,
		parser_state* state)
{
	assert(m);
	assert(st);
	assert(state);

	// get all files found in the supplied path
	const auto package_sources = m->get_package_info(path);
	assert(package_sources->sources.empty());
	assert(package_sources->load_status == package_source_info::not_loaded);
	package_sources->load_status = package_source_info::loading;
	m->load_package_sources(package_sources);
	assert(!package_sources->sources.empty());

	// application entry point functionality is always put in the root package
	const auto app_package = st->get_root_package();

	// parse each source code found
	for (auto src: package_sources->sources)
	{
		lexer l(src->get_text());
		token t(&l);
		state->set_source_code(src);

		const collision_detector detector;
		const parser_scope ps0(state, &t);
		const parser_scope ps1(&ps0, app_package);
		t.next();
		parse_package_pre_scope(&ps1);
	}
	package_sources->load_status = package_source_info::successful;
	return app_package;
}

node_package* o2::parse_module_import(array_view<source_code*> sources, string_view package_name, parser_state* state)
{
	assert(state);

	// create a new package based on the import
	auto package = o2_new node_package(source_code_view(), package_name);
	auto guard = memory_guard(package);
	// parse each source code found
	for (auto src: sources)
	{
		lexer l(src->get_text());
		token t(&l);
		state->set_source_code(src);

		collision_detector detector;
		const parser_scope ps0(state, &t);
		const parser_scope ps1(&ps0, package);
		t.next();
		parse_package_pre_scope(&ps1);
	}

	return guard.done();
}

node_package* o2::parse_module_import(syntax_tree* st, const module* m, string_view path,
		parser_state* state)
{
	assert(m);
	assert(st);
	assert(state);

	// get all files found in the supplied path. If the module is already loaded then ignore
	const auto package_sources = m->get_package_info(path);
	if (package_sources->load_status != package_source_info::not_loaded)
		return nullptr;
	package_sources->load_status = package_source_info::loading;
	m->load_package_sources(package_sources);

	// create a new package based on the import
	auto package = o2_new node_package(source_code_view(), m->get_relative_path(path));
	auto guard = memory_guard(package);
	// parse each source code found
	for (auto src: package_sources->sources)
	{
		lexer l(src->get_text());
		token t(&l);
		state->set_source_code(src);

		collision_detector detector;
		const parser_scope ps0(state, &t);
		const parser_scope ps1(&ps0, st->get_root_package());
		const parser_scope ps2(&ps1, package);
		t.next();
		parse_package_pre_scope(&ps2);
	}
	package_sources->load_status = package_source_info::successful;
	return guard.done();
}

void o2::optimize(syntax_tree* st, int level)
{
	// TODO: Add maximum iterations?

	if (level >= 0)
	{
		while (true)
		{
			node_optimizer_binop_merge opt0_0;
			node_optimizer_unaryop_merge opt0_1;
			st->optimize(&opt0_0);
			st->optimize(&opt0_1);

			if (opt0_0.count == 0 && opt0_1.count == 0)
				break;
			opt0_0.count = 0;
			opt0_1.count = 0;
		}
	}
}
