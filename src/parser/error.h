//
// Part of the o2 Project, under the Apache License v2.0 with o2 Project Exceptions.
// See the LICENSE file in the project root for license terms
//

#pragma once

#include <exception>
#include <sstream>
#include <string_view>
#include <string>
#include "token.h"
#include "source_code_view.h"

namespace o2
{
	enum class error_types
	{
		parser_errors_starts,
		expected_identity,
		expected_constant,
		expected_type,
		unexpected_extern_func_body,
		not_implemented,
		syntax_error,
		incompatible_types,
		named_symbol_already_declared,

		structure_errors_start,
		expected_child_node,
		unexpected_child_node,

		resolve_error_starts,
		recursion,
		multiple_refs,
		positive_int_constant_expected,
		unresolved_reference,
		expected_inherits_from_attribute,
		multiple_inherited_bodies
	};

	// \brief base class for parser errors
	class error
			: public std::exception
	{
		template<typename Arg>
		static void _set(std::stringstream& ss, Arg first)
		{
			ss << first;
		}

		template<typename Arg, typename... Args>
		static void _set(std::stringstream& ss, Arg first, Args... args)
		{
			ss << first;
			_set(ss, args...);
		}

	public:
		error(const error_types type, const source_code_view& view)
				: _type(type), _view(view)
		{
		}

		// \brief helper constructor for a list of values to be part of the error
		template<typename... Args>
		error(const error_types type, const source_code_view& view, Args... args)
				: _type(type), _view(view)
		{
			stringstream ss;
			_set(ss, args...);
			set(ss.str());
		}

#pragma region std::exception

		char const* what() const noexcept final
		{
			return _error.c_str();
		}

#pragma endregion

		/**
		 * \return the error as a string
		 */
		string_view get_error() const
		{
			return _error;
		}

		/**
		 * \return an integer representation of this error
		 */
		error_types get_code() const
		{
			return _type;
		}

		// \brief helper method printing the error out to stderr
		void print(basic_ostream& stream) const;

	protected:
		void set(string error)
		{
			_error = std::move(error);
		}

	private:
		const error_types _type;
		const source_code_view _view;
		string _error;
	};

	/**
	 * \brief errors raised during the parser stage
	 */
	class parse_error
			: public error
	{
	public:
		parse_error(const error_types type, const source_code_view& view)
				: error(type, view)
		{
		}
	};

	class error_syntax_error
			: public parse_error
	{
	public:
		error_syntax_error(source_code_view view, const token* t, const char* prefix);
	};

	class error_not_implemented
			: public parse_error
	{
	public:
		error_not_implemented(source_code_view view, const token* t);

		error_not_implemented(source_code_view view, const char* extra);
	};

	class error_expected_identity
			: public parse_error
	{
	public:
		error_expected_identity(source_code_view view, const token* t);
	};

	class error_expected_constant
			: public parse_error
	{
	public:
		error_expected_constant(source_code_view view, const token* t);
	};

	class error_unexpected_extern_func_body
			: public parse_error
	{
	public:
		explicit error_unexpected_extern_func_body(const source_code_view& view);
	};

	class error_named_symbol_already_declared
			: public parse_error
	{
	public:
		error_named_symbol_already_declared(const source_code_view& view, string_view name,
				const source_code_view& collision);

		[[nodiscard]] const source_code_view& get_collision() const
		{
			return _collision;
		}

	private:
		const source_code_view _collision;
	};

	/**
	 * \brief errors raised if the node structure is invalid
	 */
	class structure_error
			: public error
	{
	public:
		structure_error(const error_types type, const source_code_view& view)
				: error(type, view)
		{
		}
	};

	/**
	 * \brief error is raised if a child-node is missing
	 */
	class expected_child_node
			: public structure_error
	{
	public:
		expected_child_node(const source_code_view& view, const char* expected_nodes);
	};

	/**
	 * \brief error is raised if a node has unexpected child-nodes
	 */
	class unexpected_child_node
			: public structure_error
	{
	public:
		unexpected_child_node(const source_code_view& view, const char* expected_nodes);
	};

	/**
	 * \brief error raised when problems occure during the resolve phase
	 */
	class resolve_error
			: public error
	{
	public:
		resolve_error(const error_types type, const source_code_view& view)
				: error(type, view)
		{
		}
	};

	class recursion_detector;

	class node;

	/**
	 * \brief error raised if recursion is detected during the resolve phase
	 */
	class resolve_error_recursion
			: public resolve_error
	{
	public:
		resolve_error_recursion(source_code_view view, const node* n, const recursion_detector* rd);
	};

	/**
	 * \brief error raised when resolving a reference could not identify one single unique reference
	 */
	class resolve_error_multiple_refs
			: public resolve_error
	{
	public:
		explicit resolve_error_multiple_refs(const source_code_view& view);
	};

	/**
	 * \brief error raised if a reference could not be resolved
	 */
	class resolve_error_unresolved_reference
			: public resolve_error
	{
	public:
		explicit resolve_error_unresolved_reference(const source_code_view& view);
	};

	/**
	 * \brief error raised when a positive constant should be resolved but wasn't
	 */
	class resolve_error_positive_int_constant_expected
			: public resolve_error
	{
	public:
		explicit resolve_error_positive_int_constant_expected(const source_code_view& view);
	};

	/**
	 * \brief error raised when a declared attribute statement is't an attribute type
	 */
	class resolve_error_expected_inherits_from_attribute
			: public resolve_error
	{
	public:
		explicit resolve_error_expected_inherits_from_attribute(const source_code_view& view);
	};

	/**
	 * \brief error raised when a type inherits from more than type with a body
	 */
	class resolve_error_multiple_inherited_bodies
			: public resolve_error
	{
	public:
		explicit resolve_error_multiple_inherited_bodies(const source_code_view& view);
	};
}

namespace std
{
	inline ostream& operator<<(ostream& os, const o2::error_types& et)
	{
		switch (et)
		{
		case o2::error_types::expected_identity:
			return os << "expected_identity";
		case o2::error_types::expected_constant:
			return os << "expected_constant";
		case o2::error_types::expected_type:
			return os << "expected_type";
		case o2::error_types::not_implemented:
			return os << "not_implemented";
		case o2::error_types::syntax_error:
			return os << "syntax_error";
		case o2::error_types::incompatible_types:
			return os << "incompatible_types";
		case o2::error_types::named_symbol_already_declared:
			return os << "named_symbol_already_declared";
		case o2::error_types::recursion:
			return os << "recursion";
		case o2::error_types::unresolved_reference:
			return os << "unresolved_reference";
		case o2::error_types::unexpected_extern_func_body:
			return os << "unexpected_extern_func_body";
		case o2::error_types::multiple_refs:
			return os << "multiple_refs";
		case o2::error_types::expected_child_node:
			return os << "expected_child_node";
		case o2::error_types::unexpected_child_node:
			return os << "unexpected_child_node";
		case o2::error_types::positive_int_constant_expected:
			return os << "positive_int_constant_expected";
		case o2::error_types::expected_inherits_from_attribute:
			return os << "expected_inherits_from_attribute";
		case o2::error_types::multiple_inherited_bodies:
			return os << "multiple_inherited_bodies";
		default:
			return os << "???";
		}
	}
}