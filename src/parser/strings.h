//
// Part of the o2 Project, under the Apache License v2.0 with o2 Project Exceptions.
// See the LICENSE file in the project root for license terms
//

#pragma once

#include <cinttypes>

namespace o2
{
	/**
	* \brief compare two non-null terminated strings with known sizes
	* \param s1 the first string
	* \param l1 the length of the first string
	* \param s2 the second string
	* \param l2 the length of the second string
	**/
	static bool w_streqn(const char* s1, int l1, const char* s2, int l2)
	{
		if (l1 != l2) 
			return false;
		const char* end = s1 + l1;
		for (; s1 != end;) {
			if (*s1++ != *s2++)
				return false;
		}
		return true;
	}

	/**
	* \brief convert a string to an 64bit integer
	* \param str the string
	* \param len the length of the string
	**/
	static std::int64_t w_strtoi64(const char* str, int len)
	{
		std::int64_t ret = 0;
		std::int64_t multiplier = 1;
		if (len > 0 && *str == '-')
		{
			multiplier = -1;
			len--;
			str++;
		}
		for (int i = 0; i < len; ++i)
			ret = (ret * static_cast<std::int64_t>(10)) + static_cast<std::int64_t>(str[i] - '0');
		return ret * multiplier;
	}

	/**
	* \brief convert a string to an unsigned 64bit integer
	* \param str the string
	* \param len the length of the string
	**/
	static std::uint64_t w_strtou64(const char* str, int len)
	{
		std::uint64_t ret = 0;
		const auto end = str + len;
		for (; str != end; ++str)
		{
			if (*str == 'u') break;
			ret = (ret * static_cast<std::uint64_t>(10)) + static_cast<std::uint64_t>(*str - '0');
		}
		return ret;
	}

	/**
	 * \brief convert a hex key into an integer
	 * \param c the hex key
	 * \return the integer
	 */
	static int w_hkeytoint(const char c)
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
			return c - '0';
		case 'a':
		case 'b':
		case 'c':
		case 'd':
		case 'e':
		case 'f':
			return c - 'a' + 10;
		case 'A':
		case 'B':
		case 'C':
		case 'D':
		case 'E':
		case 'F':
			return c - 'A' + 10;
		default:
			return 0;
		}
	}

	/**
	 * \brief convert a constant hex string into a 64bit value
	 * \param hex 
	 * \param len 
	 * \return 
	 */
	static std::int64_t w_hextoi64(const char* hex, int len)
	{
		std::int64_t ret = 0;
		const char* end = hex + len;
		if (len < 3)
			return 0;
		hex += 2;
		for (; hex != end; ++hex)
			ret = (ret * 16) + static_cast<std::int64_t>(w_hkeytoint(*hex));
		return ret;
	}

	/**
	 * \brief convert a constant hex string into a unsigned 64bit value
	 * \param hex
	 * \param len
	 * \return
	 */
	std::uint64_t w_hextou64(const char* hex, int len)
	{
		std::uint64_t ret = 0;
		const char* end = hex + len;
		if (len < 3)
			return 0;
		hex += 2;
		for (; hex != end; ++hex)
			ret = (ret * 16) + static_cast<std::uint64_t>(w_hkeytoint(*hex));
		return ret;
	}

}