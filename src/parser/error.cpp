//
// Part of the o2 Project, under the Apache License v2.0 with o2 Project Exceptions.
// See the LICENSE file in the project root for license terms
//

#include "error.h"
#include "recursion_detector.h"
#include "node_symbol.h"
#include <iomanip>
#include <algorithm>

using namespace o2;

void error::print(basic_ostream& stream) const
{
	auto src = _view.get_source_code();

	stream << "Failed to compile " << src->get_filename() << ": " << std::endl;

	auto text = src->get_text();
	const string_literal* s = text.data();
	const string_literal* end = s + text.length();
	int line = 0;
	for (; s != end; ++s)
	{
		stream << *s;
		if (*s == '\n')
		{
			if ((++line) == _view.get_line() + 1)
			{
				for (int i = 0; i < _view.get_line_offset() - 1; ++i)
				{
					if (_view.get_line_start()[i] == '\t')
						stream << '\t';
					else
						stream << ' ';
				}
				stream << "^ " << std::setfill(STR('0')) << std::setw(6) << (int)_type;
				stream << " " << what() << std::endl;
			}
		}
	}
	if ((++line) == _view.get_line() + 1)
	{
		stream << std::endl;
		for (int i = 0; i < _view.get_line_offset() - 1; ++i)
		{
			if (_view.get_line_start()[i] == '\t')
				stream << '\t';
			else
				stream << ' ';
		}
		stream << "^ " << std::setfill(STR('0')) << std::setw(6) << (int)_type;
		stream << " " << what() << std::endl;
	}
}

error_syntax_error::error_syntax_error(source_code_view view, const token* t, const char* prefix)
		: parse_error(error_types::syntax_error, view)
{
	stringstream s;
	s << prefix << " but was ";
	if (t->type() == token_type::eof)
		s << "<EOF>";
	else
		s << "'" << t->value() << "'";
	set(std::move(s.str()));
}

error_not_implemented::error_not_implemented(source_code_view view, const token* t)
		: parse_error(error_types::not_implemented, view)
{
	stringstream s;
	s << "'" << t->value() << "' is not implemented yet";
	set(std::move(s.str()));
}

error_not_implemented::error_not_implemented(source_code_view view, const char* message)
		: parse_error(error_types::not_implemented, view)
{
	stringstream s;
	s << message << " are not implemented yet";
	set(std::move(s.str()));
}

error_expected_identity::error_expected_identity(source_code_view view, const token* t)
		: parse_error(error_types::expected_identity, view)
{
	stringstream s;
	s << "expected 'identity' but was ";
	if (t->type() == token_type::eof)
		s << "<EOF>";
	else
		s << "'" << t->value() << "'";
	set(std::move(s.str()));
}

error_expected_constant::error_expected_constant(source_code_view view, const token* t)
		: parse_error(error_types::expected_constant, view)
{
	stringstream s;
	s << "expected a constant but was ";
	if (t->type() == token_type::eof)
		s << "<EOF>";
	else
		s << "'" << t->value() << "'";
	set(std::move(s.str()));
}

error_unexpected_extern_func_body::error_unexpected_extern_func_body(const source_code_view& view)
		: parse_error(error_types::unexpected_extern_func_body, view)
{
	stringstream s;
	s << "unexpected extern function body";
	set(std::move(s.str()));
}

error_named_symbol_already_declared::error_named_symbol_already_declared(const source_code_view& view, string_view name,
		const source_code_view& collision)
		: parse_error(error_types::named_symbol_already_declared, view), _collision(collision)
{
	stringstream s;
	s << "symbol '" << name << "' is already declared";
	set(std::move(s.str()));
}

expected_child_node::expected_child_node(const source_code_view& view, const char* expected_nodes)
		: structure_error(error_types::expected_child_node, view)
{
	stringstream s;
	s << "node doesn't have the necessary child nodes: " << expected_nodes;
	set(std::move(s.str()));
}

unexpected_child_node::unexpected_child_node(const source_code_view& view, const char* expected_nodes)
		: structure_error(error_types::unexpected_child_node, view)
{
	stringstream s;
	s << "node expected child node of type " << expected_nodes << " but wasn't";
	set(std::move(s.str()));
}

resolve_error_recursion::resolve_error_recursion(source_code_view view, const node* n, const recursion_detector* rd)
		: resolve_error(error_types::recursion, view)
{
	stringstream s;
	s << "recursion detected: '";

	rd = rd->first_of_type<node_symbol>();
	if (rd)
	{
		stringstream recursion;

		auto symbol = dynamic_cast<const node_symbol*>(n);
		if (symbol)
		{
			auto id = static_cast<const node_symbol*>(n)->get_id();
			for (auto c = id.rbegin(); c != id.rend(); ++c)
				recursion << *c;
			recursion << ">-";
		}

		auto id = static_cast<const node_symbol*>(rd->n)->get_id();
		for (auto c = id.rbegin(); c != id.rend(); ++c)
			recursion << *c;
		rd = rd->next_of_type<node_symbol>();
		while (rd)
		{
			recursion << ">-";
			id = static_cast<const node_symbol*>(rd->n)->get_id();
			for (auto c = id.rbegin(); c != id.rend(); ++c)
				recursion << *c;
			if (rd->n == n)
				break;
			rd = rd->next_of_type<node_symbol>();
		}

		string str = recursion.str();
		std::reverse(str.rbegin(), str.rend());
		s << str;
		s << "'";
	}
	set(std::move(s.str()));
}

resolve_error_multiple_refs::resolve_error_multiple_refs(const source_code_view& view)
		: resolve_error(error_types::multiple_refs, view)
{
	stringstream s;
	s << "multiple references found";
	set(std::move(s.str()));
}

resolve_error_unresolved_reference::resolve_error_unresolved_reference(const source_code_view& view)
		: resolve_error(error_types::unresolved_reference, view)
{
	stringstream s;
	s << "unresolved reference";
	set(std::move(s.str()));
}

resolve_error_positive_int_constant_expected::resolve_error_positive_int_constant_expected(const source_code_view& view)
		: resolve_error(error_types::positive_int_constant_expected, view)
{
	stringstream s;
	s << "a positive constant was expected";
	set(std::move(s.str()));
}

resolve_error_expected_inherits_from_attribute::resolve_error_expected_inherits_from_attribute(
		const source_code_view& view)
		: resolve_error(error_types::expected_inherits_from_attribute, view)
{
	stringstream s;
	s << "expected attribute statement to extend from stdlib.attribute";
	set(std::move(s.str()));
}

resolve_error_multiple_inherited_bodies::resolve_error_multiple_inherited_bodies(const source_code_view& view)
		: resolve_error(error_types::multiple_inherited_bodies, view)
{
	stringstream s;
	s << "type inherits from more than one type with a body";
	set(std::move(s.str()));
}
