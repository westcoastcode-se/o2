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

void error::print(std::basic_ostream<char>& stream) const
{
	auto src = _view.get_source_code();

	stream << "Failed to compile " << src->get_filename() << ": " << std::endl;

	auto text = src->get_text();
	const char* s = text.data();
	const char* end = s + text.length();
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
				stream << "^ " << std::setfill('0') << std::setw(6) << (int)_type;
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
		stream << "^ " << std::setfill('0') << std::setw(6) << (int)_type;
		stream << " " << what() << std::endl;
	}
}

error_syntax_error::error_syntax_error(source_code_view view, const token* t, const char* prefix)
		: parse_error(error_types::syntax_error, view)
{
	std::stringstream s;
	s << prefix << " but was ";
	if (t->type() == token_type::eof)
		s << "<EOF>";
	else
		s << "'" << t->value() << "'";
	set(s.str());
}

error_not_implemented::error_not_implemented(source_code_view view, const token* t)
		: parse_error(error_types::not_implemented, view)
{
	std::stringstream s;
	s << "'" << t->value() << "' is not implemented yet";
	set(s.str());
}

error_not_implemented::error_not_implemented(source_code_view view, const char* message)
		: parse_error(error_types::not_implemented, view)
{
	std::stringstream s;
	s << message << " is not implemented yet";
	set(s.str());
}

error_expected_identity::error_expected_identity(source_code_view view, const token* t)
		: parse_error(error_types::expected_identity, view)
{
	std::stringstream s;
	s << "expected 'identity' but was ";
	if (t->type() == token_type::eof)
		s << "<EOF>";
	else
		s << "'" << t->value() << "'";
	set(s.str());
}

error_expected_constant::error_expected_constant(source_code_view view, const token* t)
		: parse_error(error_types::expected_constant, view)
{
	std::stringstream s;
	s << "expected a constant but was ";
	if (t->type() == token_type::eof)
		s << "<EOF>";
	else
		s << "'" << t->value() << "'";
	set(s.str());
}

error_unexpected_extern_func_body::error_unexpected_extern_func_body(source_code_view view)
		: parse_error(error_types::unexpected_extern_func_body, view)
{
	std::stringstream s;
	s << "unexpected extern function body";
	set(s.str());
}

error_named_symbol_already_declared::error_named_symbol_already_declared(source_code_view view, string_view name)
		: parse_error(error_types::named_symbol_already_declared, view)
{
	std::stringstream s;
	s << "symbol '" << name << "' is already declared";
	set(s.str());
}

resolve_error_recursion::resolve_error_recursion(source_code_view view, const node* n, const recursion_detector* rd)
		: resolve_error(error_types::recursion, view)
{
	std::stringstream s;
	s << "recursion detected: '";

	rd = rd->first_of_type<node_symbol>();
	if (rd)
	{
		std::stringstream recursion;

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
			rd = rd->next_of_type<node_symbol>();
		}

		std::string str = recursion.str();
		std::reverse(str.rbegin(), str.rend());
		s << str;
		s << "'";
	}
	set(s.str());
}

resolve_error_multiple_refs::resolve_error_multiple_refs(source_code_view view)
		: resolve_error(error_types::multiple_refs, view)
{
	std::stringstream s;
	s << "multiple references found";
	set(s.str());
}

resolve_error_unresolved_reference::resolve_error_unresolved_reference(source_code_view view)
		: resolve_error(error_types::unresolved_reference, view)
{
	std::stringstream s;
	s << "unresolved reference";
	set(s.str());
}
