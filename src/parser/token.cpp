//
// Part of the o2 Project, under the Apache License v2.0 with o2 Project Exceptions.
// See the LICENSE file in the project root for license terms
//

#include "token.h"
#include "strings.h"
#include <cmath>
#include <cfloat>

using namespace o2;

namespace
{
	inline bool is_keyword_start(string_view::value_type c)
	{
		switch (c)
		{
		case 'a':
		case 'b':
		case 'c':
		case 'd':
		case 'e':
		case 'f':
		case 'g':
		case 'h':
		case 'i':
		case 'j':
		case 'k':
		case 'l':
		case 'm':
		case 'n':
		case 'o':
		case 'p':
		case 'q':
		case 'r':
		case 's':
		case 't':
		case 'u':
		case 'v':
		case 'w':
		case 'x':
		case 'y':
		case 'z':
		case 'A':
		case 'B':
		case 'C':
		case 'D':
		case 'E':
		case 'F':
		case 'G':
		case 'H':
		case 'I':
		case 'J':
		case 'K':
		case 'L':
		case 'M':
		case 'N':
		case 'O':
		case 'P':
		case 'Q':
		case 'R':
		case 'S':
		case 'T':
		case 'U':
		case 'V':
		case 'W':
		case 'X':
		case 'Y':
		case 'Z':
		case '_':
			return true;
		default:
			return false;
		}
	}

	inline bool is_digit(string_view::value_type c)
	{
		switch (c)
		{
		case '0':
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9':
			return true;
		default:
			return false;
		}
	}

	inline bool is_keyword_mid(string_view::value_type c)
	{
		return is_keyword_start(c) || is_digit(c);
	}

	inline bool is_hex(string_view::value_type c)
	{
		switch (c)
		{
		case '0':
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9':
		case 'a':
		case 'b':
		case 'c':
		case 'd':
		case 'e':
		case 'f':
		case 'A':
		case 'B':
		case 'C':
		case 'D':
		case 'E':
		case 'F':
			return true;
		default:
			return false;
		}
	}


	inline bool is_whitespace(string_view::value_type c)
	{
		switch (c)
		{
		case ' ':
		case '\t':
		case '\r':
			return true;
		default:
			return false;
		}
	}

	inline bool is_newline(string_view::value_type c)
	{
		return c == '\n';
	}
}

int token::line_offset() const
{
	return (int)(_pos - _line_offset);
}

int token::offset() const
{
	return (int)(_pos - _lexer->first());
}

token_type token::next()
{
	_modifier = token_modifier::none;
	auto c = *_pos;
	if (c == 0)
		return eof();

	while (is_whitespace(c))
		c = *++_pos;
	next0();
	return _type;
}

token_type token::next_until(token_type t1)
{
	token_type res = next();
	while (res != t1)
		res = next();
	return res;
}

token_type token::next_until(token_type t1, token_type t2)
{
	token_type res = next();
	while (res != t1 && res != t2)
		res = next();
	return res;
}

token_type token::next_until_not(token_type t1)
{
	token_type res = next();
	while (res == t1)
		res = next();
	return res;
}

token_type token::next_until_not(token_type t1, token_type t2)
{
	token_type res = next();
	while (res == t1 || res == t2)
		res = next();
	return res;
}

token_type token::next_until_not(token_type t1, token_type t2, token_type t3)
{
	token_type res = next();
	while (res == t1 || res == t2 || res == t3)
		res = next();
	return res;
}

std::int64_t token::value_int64() const
{
	if (_type == token_type::hex)
		return w_hextoi64(_string_start, static_cast<int>(_string_end - _string_start));
	return w_strtoi64(_string_start, static_cast<int>(_string_end - _string_start));
}

std::uint64_t token::value_uint64() const
{
	if (_type == token_type::hex)
		return w_hextou64(_string_start, static_cast<int>(_string_end - _string_start));
	return w_strtou64(_string_start, static_cast<int>(_string_end - _string_start));
}

bool token::value_bool() const
{
	return w_streqn(_string_start, static_cast<int>(_string_end - _string_start), "true", 4);
}

float token::value_float32() const
{
	char temp[64];
	temp[sprintf(temp, "%.*s", static_cast<int>(_string_end - _string_start), _string_start)] = 0;

	const float v = strtof(temp, NULL);
	if (v < (FLT_MAX - FLT_EPSILON) && v > (FLT_MIN + FLT_EPSILON))
		return v;
	else
		return 0.0f;
}

double token::value_float64() const
{
	char temp[64];
	temp[sprintf(temp, "%.*s", static_cast<int>(_string_end - _string_start), _string_start)] = 0;

	const double v = strtod(temp, NULL);
	if (v < (DBL_MAX - DBL_EPSILON) && v > (DBL_MIN + DBL_EPSILON))
		return v;
	else
		return 0.0;
}

void token::next0()
{
	const auto c = *_pos;

	// Is this a potential keyword or identity?
	if (is_keyword_start(c))
	{
		next_keyword();
		return;
	}

	// Is this a potential number?
	if (is_digit(c))
	{
		next_number();
		return;
	}

	switch (c)
	{
	case '(':
		atom(token_type::parant_left);
		break;
	case ')':
		atom(token_type::parant_right);
		break;
	case '[':
		if (peek(1) == '[')
		{
			_pos++;
			atom(token_type::attribute_left);
			break;
		}
		atom(token_type::square_left);
		break;
	case ']':
		if (peek(1) == ']')
		{
			_pos++;
			atom(token_type::attribute_right);
			break;
		}
		atom(token_type::square_right);
		break;
	case '{':
		atom(token_type::bracket_left);
		break;
	case '&':
		atom(token_type::ref);
		break;
	case '*':
		atom(token_type::pointer);
		break;
	case '}':
		atom(token_type::bracket_right);
		break;
	case '.':
		atom(token_type::dot);
		break;
	case ',':
		atom(token_type::comma);
		break;
	case '>':
		next_gt_or_gte();
		break;
	case '<':
		next_lt_or_lte();
		break;
	case '=':
		if (peek(1) == '=')
		{
			_pos++;
			atom(token_type::test_equals);
			break;
		}
		atom(token_type::assign);
		break;
	case '~':
		atom(token_type::bit_not);
		break;
	case ';':
		atom(token_type::semicolon);
		break;
	case ':':
		if (peek(1) == ':')
		{
			_pos++;
			atom(token_type::separator);
			break;
		}
		atom(token_type::colon);
		break;
	case '@':
		next_attribute();
		break;
	case '"':
		next_string('"');
		break;
	case '`':
		next_string('`');
		break;
	case '\\':
		atom(token_type::escape);
		break;
	case '/':
		next_comment_or_div();
		break;
	case '!':
		next_not_or_not_equals();
		break;
	case '+':
		next_plus_or_inc();
		break;
	case '|':
		atom(token_type::bit_or);
		break;
	case '^':
		atom(token_type::bit_xor);
		break;
	case '-':
		next_minus_or_dec();
		break;
	case '\n':
		atom(token_type::newline);
		_line++;
		_line_offset = _pos;
		break;
	case 0:
		eof();
		break;
	default:
		unknown();
		break;
	}
}

void token::atom(token_type type)
{
	_type = type;
	_string_start = _pos;
	_string_end = ++_pos;
}

void token::unknown()
{
	_type = token_type::unknown;
	_string_start = _pos;
	_string_end = ++_pos;
}

token_type token::eof()
{
	_string_start = _string_end = _lexer->last();
	_type = token_type::eof;
	return _type;
}

string_view::value_type token::peek(int n) const
{
	return *(_pos + n);
}

void token::next_string(char c)
{
	const string_view::value_type* start = ++_pos;
	if (*start == 0)
	{
		eof();
		return;
	}

	bool escaped = false;
	while (*_pos != 0)
	{
		if (*_pos == '\\' && !escaped)
		{
			escaped = true;
			_pos++;
			continue;
		}

		if (escaped)
		{
			escaped = false;
			_pos++;
			continue;
		}

		if (*_pos == c && !escaped)
		{
			break;
		}
		_pos++;
	}
	_string_start = start;
	_string_end = _pos;
	_pos++;
	_type = token_type::string;
	if (value_length() == 0)
		_modifier = token_modifier::hint_empty;
}

void token::next_number()
{
	// Remember the first chacter and step to next ch
	const auto start = _pos;
	_pos++;

	// Ignore all digits
	while (is_digit(*_pos)) _pos++;

	// Time to figure out if this is an integer, decimal or a hex value
	_type = token_type::number;

	// Is this is a decimal?
	if (*_pos == '.')
	{
		_type = token_type::decimal;
		_pos++;

		// Ignore all digits
		while (is_digit(*_pos)) _pos++;

		// This might be a value with format -3.402823466e+38f
		if (*_pos == 'e')
		{
			string_view::value_type peek = *(_pos + 1);
			if (peek == '+' || peek == '-')
			{
				_pos += 2;

				// Ignore all numbers
				while (is_digit(*_pos)) _pos++;
			}
		}

		if (*_pos == 'f')
		{
			_modifier = token_modifier::hint_float;
			_pos++;
		}

		_string_start = start;
		_string_end = _pos;
		return;
	}

	// Hex-decimal value (must start with a 0)
	if (*start == '0' && *_pos == 'x')
	{
		_type = token_type::hex;
		_pos++;

		// Ignore all numbers
		while (is_hex(*_pos)) _pos++;
	}

	// Hint that this value is an unsigned constant
	if (*_pos == 'u')
	{
		_modifier = token_modifier::hint_unsigned;
		_pos++;
	}

	_string_start = start;
	_string_end = _pos;
}

void token::next_keyword()
{
	const auto start = _pos++;

	// Ignore all chacters
	while (is_keyword_mid(*_pos)) _pos++;

	_string_start = start;
	_string_end = _pos;
	_type = hint_keyword_type();
	if (value_length() == 1 && *_string_start == '_')
		_modifier = token_modifier::hint_empty;
}

void token::next_attribute()
{
	const string_view::value_type* start = ++_pos;
	if (*start == 0)
	{
		eof();
		return;
	}

	bool escaped = false;
	while (*_pos != 0)
	{
		if (*_pos == '\\' && !escaped)
		{
			escaped = true;
			_pos++;
			continue;
		}

		if (escaped)
		{
			escaped = false;
			_pos++;
			continue;
		}

		if (*_pos == '\n' && !escaped)
		{
			break;
		}
		if (*_pos == '\n')
		{
			_line++;
			_line_offset = _pos + 1;
		}
		_pos++;
	}
	_string_start = start;
	_string_end = _pos;
	_pos++;
	_type = token_type::attribute;
}

void token::next_comment_or_div()
{
	auto n = peek(1);
	if (n == '/')
	{
		const string_view::value_type* start = ++_pos;
		while (*_pos != 0 && *_pos != '\n') _pos++;
		_string_start = start + 1;
		_string_end = _pos - 1;
		_type = token_type::comment;
		return;
	}

	if (n != '*')
	{
		atom(token_type::div);
		return;
	}

	const string_view::value_type* start = ++_pos;
	if (_pos != 0)
		start++;
	while (*_pos != 0)
	{
		if (*_pos != '*')
		{
			_pos++;
			continue;
		}

		if (peek(1) == '/')
		{
			_pos++;
			break;
		}
		_pos++;
	}

	_string_start = start;
	_string_end = _pos - 1;
	_pos++;
	_type = token_type::comment;
}

void token::next_minus_or_dec()
{
	if (peek(1) == '-')
	{
		_pos++;
		atom(token_type::dec);
		return;
	}

	atom(token_type::minus);
}

void token::next_plus_or_inc()
{
	if (peek(1) == '+')
	{
		_pos++;
		atom(token_type::inc);
		return;
	}

	atom(token_type::plus);
}

void token::next_lt_or_lte()
{
	if (peek(1) == '=')
	{
		_pos++;
		atom(token_type::test_lte);
		return;
	}

	atom(token_type::test_lt);
}

void token::next_gt_or_gte()
{
	if (peek(1) == '=')
	{
		_pos++;
		atom(token_type::test_gte);
		return;
	}

	atom(token_type::test_gt);
}

void token::next_not_or_not_equals()
{
	if (peek(1) == '=')
	{
		_pos++;
		atom(token_type::test_not_equals);
		return;
	}

	atom(token_type::not_);
}

token_type token::hint_keyword_type() const
{
	const auto str = value();
	const auto len = str.length();

	if (len == 2)
	{
		static const string_view IF("if");
		static const string_view AS("as");

		if (str == IF) return token_type::if_;
		if (str == AS) return token_type::as;
	}
	else if (len == 3)
	{
		static const string_view FOR("for");
		static const string_view VAR("var");

		if (str == FOR) return token_type::for_;
		if (str == VAR) return token_type::var;
	}
	else if (len == 4)
	{
		static const string_view FUNC("func");
		static const string_view TYPE("type");
		static const string_view ELSE("else");
		static const string_view CAST("cast");
		static const string_view TRUE_("true");

		if (str == FUNC) return token_type::func;
		if (str == TYPE) return token_type::type;
		if (str == ELSE) return token_type::else_;
		if (str == CAST) return token_type::cast;
		if (str == TRUE_) return token_type::boolean;
	}
	else if (len == 5)
	{
		static const string_view CONST("const");
		static const string_view FINAL("final");
		static const string_view FALSE_("false");
		static const string_view TRAIT("trait");

		if (str == CONST) return token_type::const_;
		if (str == FINAL) return token_type::final_;
		if (str == FALSE_) return token_type::boolean;
		if (str == TRAIT) return token_type::trait;
	}
	else if (len == 6)
	{
		static const string_view PUBLIC("public");
		static const string_view STATIC("static");
		static const string_view INLINE("inline");
		static const string_view IMPORT("import");
		static const string_view RETURN("return");
		static const string_view STRUCT("struct");
		static const string_view EXTERN("extern");

		if (str == PUBLIC) return token_type::public_;
		if (str == STATIC) return token_type::static_;
		if (str == INLINE) return token_type::inline_;
		if (str == IMPORT) return token_type::import;
		if (str == RETURN) return token_type::return_;
		if (str == STRUCT) return token_type::struct_;
		if (str == EXTERN) return token_type::extern_;
	}
	else if (len == 7)
	{
		static const string_view PRIVATE("private");
		static const string_view TYPEDEF("typedef");

		if (str == PRIVATE) return token_type::private_;
		if (str == TYPEDEF) return token_type::typedef_;
	}
	else if (len == 8)
	{
		static const string_view OVERRIDE("override");

		if (str == OVERRIDE) return token_type::override_;
	}
	else if (len == 9)
	{
		static const string_view INTERFACE("interface");

		if (str == INTERFACE) return token_type::interface;
	}

	return token_type::identity;
}
