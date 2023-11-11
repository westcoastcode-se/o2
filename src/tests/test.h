//
// Part of the o2 Project, under the Apache License v2.0 with o2 Project Exceptions.
// See the LICENSE file in the project root for license terms
//

#pragma once

#include <sstream>
#include <exception>
#include <functional>
#include <iostream>
#include <string_view>
#include <cstdarg>
#include <filesystem>
#include <chrono>

#if defined(_WIN32) || defined(_WIN64)

#include <Windows.h>

#endif

namespace o2::testing
{
	struct colors
	{
#if defined(_WIN32) || defined(_WIN64)

		static inline const char* red()
		{
			set_color(FOREGROUND_RED);
			return "";
		}

		static inline const char* end()
		{
			set_color(*get_attribute());
			return "";
		}

		static inline const char* green()
		{
			set_color(FOREGROUND_GREEN);
			return "";
		}

	private:
		static inline WORD* get_attribute()
		{
			static WORD w = 0;
			return &w;
		}

		static inline void set_color(WORD Colour)
		{
			CONSOLE_SCREEN_BUFFER_INFO Info;
			HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
			GetConsoleScreenBufferInfo(hStdout, &Info);
			*get_attribute() = Info.wAttributes;
			SetConsoleTextAttribute(hStdout, Colour);
		}

#else
		static inline constexpr const char* red()
		{
			return "\033[1;31m";
		}

		static inline constexpr const char* end()
		{
			return "\033[0m";
		}

		static inline constexpr const char* green()
		{
			return "\033[1;32m";
		}
#endif
	};

	class assertion_error
			: public std::runtime_error
	{
	public:
		assertion_error(const char* str)
				: runtime_error(str)
		{
		}
	};

	/**
	 * \brief stringstream that can be thrown as an exception
	 */
	struct error_stringstream : public std::stringstream
	{
		inline void do_throw() const
		{
			throw assertion_error(str().c_str());
		}
	};

	/**
	 * \brief state used by the test driver
	 */
	struct test_state
	{
		static bool& success()
		{
			static bool _s = true;
			return _s;
		}

		static int& count()
		{
			static int _c = 0;
			return _c;
		}

		static int& failed()
		{
			static int _f = 0;
			return _f;
		}

		/**
		 * \return true if debugging mode is enabled
		 */
		static bool& debugging()
		{
			static bool d = false;
			return d;
		}

		/**
		 * \return if a suite should stop running tests as soon as one test fails
		 */
		static bool& stop_suit_on_error()
		{
			static bool b = false;
			return b;
		}

		/**
		 * \return the name of the test we want to run. empty means all
		 */
		static std::string_view& test_name()
		{
			static std::string_view s;
			return s;
		}

		static bool is_suite_ignored(std::string_view name)
		{
			if (test_name().empty())
				return false;
			const auto idx = test_name().find('.');
			bool ignored = true;
			if (idx == std::string_view::npos)
			{
				if (test_name() == name)
					ignored = false;
			}
			else if (idx > 0)
			{
				if (test_name().substr(0, idx) == name)
					ignored = false;
			}
			return ignored;
		}

		static bool is_test_ignored(std::string_view name)
		{
			if (test_name().empty())
				return false;
			const auto idx = test_name().find('.');
			if (idx == std::string_view::npos)
				return false;
			bool ignored = true;
			if (idx > 0)
			{
				if (test_name().substr(idx + 1) == name)
					ignored = false;
			}
			return ignored;
		}
	};

	static error_stringstream error()
	{
		return {};
	}

	static void throw_(const error_stringstream& stream)
	{
		stream.do_throw();
	}

	static void throw_(const char* format, ...)
	{
		char memory[4096];
		va_list argptr;
		va_start(argptr, format);
		vsprintf(memory, format, argptr);
		va_end(argptr);
		throw std::runtime_error(memory);
	}

	template<typename T, typename U>
	inline void assert_equals(T value, U expected)
	{
		if (value != expected)
		{
			throw_(error() << "assertion: expected " << expected << " but was " << value);
		}
	}

	template<typename T, typename U>
	inline void assert_not_equals(T value, U expected)
	{
		if (value == expected)
		{
			throw_(error() << "assertion: expected " << value << " to not be " << expected);
		}
	}

	inline void fail()
	{
		throw_(error() << "assertion: failure");
	}

	inline void fail(std::string_view msg)
	{
		throw_(error() << "assertion: failure: '" << msg << "'");
	}

	template<typename T>
	inline T* assert_not_null(T* ptr)
	{
		if (ptr == nullptr)
			throw_(error() << "assertion: expected ptr to not be nullptr");
		return ptr;
	}

	template<typename T>
	inline void assert_null(T* ptr)
	{
		if (ptr != nullptr)
			throw_(error() << "assertion: expected ptr to be nullptr");
	}

	inline void assert_false(bool value)
	{
		if (value)
			throw_(error() << "assertion: expected false");
	}

	inline void assert_true(bool value)
	{
		if (!value)
			throw_(error() << "assertion: expected true");
	}

	template<class H, class T>
	inline H* assert_type(T* in)
	{
		assert_not_null(in);
		if (dynamic_cast<H*>(in) == nullptr)
		{
			throw_(error() << typeid(T).name() << " is not " << typeid(H).name());
		}
		return static_cast<H*>(in);
	}

	template<class H, class T>
	inline const H* assert_type(const T* in)
	{
		if (dynamic_cast<const H*>(in) == nullptr)
		{
			throw_(error() << typeid(*in).name() << " is not " << typeid(H).name());
		}
		return static_cast<const H*>(in);
	}

	struct utils
	{
		static inline unsigned long long now()
		{
			return std::chrono::duration_cast<std::chrono::milliseconds>(
					std::chrono::system_clock::now().time_since_epoch()).count();
		}
	};

	static inline void test(std::string_view name, std::function<void()> t)
	{
		std::cout << "\ttest '" << name << "\' - ";
		if (test_state::is_test_ignored(name))
		{
			std::cout << "ignored" << std::endl;
			return;
		}
		try
		{
			const auto now = utils::now();
			t();
			const auto duration = utils::now() - now;
			std::cout << colors::green() << "ok (" << duration << " ms)" << colors::end() << std::endl;
		}
		catch (const std::exception& e)
		{
			std::cout << colors::red() << "error '" << e.what() << "'" << colors::end() << std::endl;
			test_state::failed()++;
			test_state::success() = false;
			if (test_state::stop_suit_on_error())
				throw;
		}
	}

	static inline void test(std::string_view name, std::function<void()> before, std::function<void()> t)
	{
		std::cout << "\ttest '" << name << "\' - ";
		if (test_state::is_test_ignored(name))
		{
			std::cout << "ignored" << std::endl;
			return;
		}
		try
		{
			before();
			const auto now = utils::now();
			t();
			const auto duration = utils::now() - now;
			std::cout << colors::green() << "ok (" << duration << " ms)" << colors::end() << std::endl;
		}
		catch (const std::exception& e)
		{
			std::cout << colors::red() << "error '" << e.what() << "'" << colors::end() << std::endl;
			test_state::failed()++;
			test_state::success() = false;
			if (test_state::stop_suit_on_error())
				throw;
		}
	}

	static inline void test(std::string_view name, std::function<void()> before,
			std::function<void()> t, std::function<void()> after)
	{
		std::cout << "\ttest '" << name << "\' - ";
		if (test_state::is_test_ignored(name))
		{
			std::cout << "ignored" << std::endl;
			return;
		}
		try
		{
			before();
			const auto now = utils::now();
			t();
			const auto duration = utils::now() - now;
			after();
			std::cout << colors::green() << "ok (" << duration << " ms)" << colors::end() << std::endl;
		}
		catch (const std::exception& e)
		{
			std::cout << colors::red() << "error '" << e.what() << "'" << colors::end() << std::endl;
			test_state::failed()++;
			test_state::success() = false;
			try
			{
				after();
			}
			catch (const std::exception& e)
			{
				std::cout << colors::red() << "\tafter '" << name << '\'' << " - error '" << e.what() << "'"
						  << colors::end() << std::endl;
				if (test_state::stop_suit_on_error())
					throw;
			}
			if (test_state::stop_suit_on_error())
				throw;
		}
	}

	static inline void suite(std::string_view name, std::function<void()> s)
	{
		std::cout << "suite '" << name << '\'' << std::endl;
		if (test_state::is_suite_ignored(name))
		{
			std::cout << "\tignored" << std::endl;
			return;
		}
		try
		{
			s();
		}
		catch (const std::exception& e)
		{
			std::cout << colors::red() << "\tstopped testing suite because of failed test - error '" << e.what() << "'"
					  << colors::end()
					  << std::endl;
		}
	}
}