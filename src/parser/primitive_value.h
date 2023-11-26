//
// Part of the o2 Project, under the Apache License v2.0 with o2 Project Exceptions.
// See the LICENSE file in the project root for license terms
//

#pragma once

#include <cinttypes>
#include <memory>
#include <ostream>

namespace o2
{
	enum class primitive_type
	{
		unknown = 0,
		int8,
		uint8,
		int16,
		uint16,
		int32,
		uint32,
		int64,
		uint64,
		float32,
		float64,
		bool_,
		ptr,
		count
	};

	struct primitive_value
	{
		union
		{
			std::int64_t i64;
			std::uint64_t u64;
			double f64;
			float f32;
			std::int32_t bool_;
			std::int32_t i32;
			std::uint32_t u32;
			std::int16_t i16;
			std::uint16_t u16;
			std::int8_t i8;
			std::uint8_t u8;
			char* ptr;
		};
		primitive_type type;

		/**
		 * \return the number of bytes the value takes on the stack
		 */
		[[nodiscard]] int get_stack_size() const noexcept
		{
			switch (type)
			{
			case primitive_type::int8:
				return sizeof(int8_t);
			case primitive_type::uint8:
				return sizeof(uint8_t);
			case primitive_type::int16:
				return sizeof(int16_t);
			case primitive_type::uint16:
				return sizeof(uint16_t);
			case primitive_type::int32:
				return sizeof(int32_t);
			case primitive_type::uint32:
				return sizeof(uint32_t);
			case primitive_type::int64:
				return sizeof(int64_t);
			case primitive_type::uint64:
				return sizeof(uint64_t);
			case primitive_type::float32:
				return sizeof(float);
			case primitive_type::float64:
				return sizeof(double);
			case primitive_type::bool_:
				return 4;
			case primitive_type::ptr:
				return sizeof(void*);
			default:
				return -1;
			}
		}

		/**
		 * \brief convert this value to an int
		 */
		[[nodiscard]] int32_t to_int32() const
		{
			switch (type)
			{
			case primitive_type::int8:
				return (int32_t)i8;
			case primitive_type::uint8:
				return (int32_t)u8;
			case primitive_type::int16:
				return (int32_t)i16;
			case primitive_type::uint16:
				return (int32_t)u16;
			case primitive_type::int32:
				return (int32_t)i32;
			case primitive_type::uint32:
				return (int32_t)u32;
			case primitive_type::int64:
				return (int32_t)i64;
			case primitive_type::uint64:
				return (int32_t)u64;
			case primitive_type::float32:
				return (int32_t)f32;
			case primitive_type::float64:
				return (int32_t)f64;
			default:
				return 0;
			}
		}

		[[nodiscard]] int64_t to_int64() const
		{
			switch (type)
			{
			case primitive_type::int8:
				return (int64_t)i8;
			case primitive_type::uint8:
				return (int64_t)u8;
			case primitive_type::int16:
				return (int64_t)i16;
			case primitive_type::uint16:
				return (int64_t)u16;
			case primitive_type::int32:
				return (int64_t)i32;
			case primitive_type::uint32:
				return (int64_t)u32;
			case primitive_type::int64:
				return (int64_t)i64;
			case primitive_type::uint64:
				return (int64_t)u64;
			case primitive_type::float32:
				return (int64_t)f32;
			case primitive_type::float64:
				return (int64_t)f64;
			default:
				return 0;
			}
		}

		[[nodiscard]] uint64_t to_uint64() const
		{
			switch (type)
			{
			case primitive_type::int8:
				return (uint64_t)i8;
			case primitive_type::uint8:
				return (uint64_t)u8;
			case primitive_type::int16:
				return (uint64_t)i16;
			case primitive_type::uint16:
				return (uint64_t)u16;
			case primitive_type::int32:
				return (uint64_t)i32;
			case primitive_type::uint32:
				return (uint64_t)u32;
			case primitive_type::int64:
				return (uint64_t)i64;
			case primitive_type::uint64:
				return (uint64_t)u64;
			case primitive_type::float32:
				return (uint64_t)f32;
			case primitive_type::float64:
				return (uint64_t)f64;
			default:
				return 0;
			}
		}

		[[nodiscard]] double to_float64() const
		{
			switch (type)
			{
			case primitive_type::int8:
				return (double)i8;
			case primitive_type::uint8:
				return (double)u8;
			case primitive_type::int16:
				return (double)i16;
			case primitive_type::uint16:
				return (double)u16;
			case primitive_type::int32:
				return (double)i32;
			case primitive_type::uint32:
				return (double)u32;
			case primitive_type::bool_:
				return (double)bool_;
			case primitive_type::int64:
				return (double)i64;
			case primitive_type::uint64:
				return (double)u64;
			case primitive_type::float32:
				return (double)f32;
			case primitive_type::float64:
				return f64;
			default:
				return 0.0;
			}
		}

		[[nodiscard]] char* to_ptr() const
		{
			switch (type)
			{
			case primitive_type::int8:
				return (char*)(size_t)i8;
			case primitive_type::uint8:
				return (char*)(size_t)u8;
			case primitive_type::int16:
				return (char*)(size_t)i16;
			case primitive_type::uint16:
				return (char*)(size_t)u16;
			case primitive_type::int32:
				return (char*)(size_t)i32;
			case primitive_type::uint32:
				return (char*)(size_t)u32;
#if _WIN32 || __x86_64__ || __ppc64__
			case primitive_type::int64:
				return (char*)i64;
			case primitive_type::uint64:
				return (char*)u64;
#else
#endif
			default:
				return nullptr;
			}
		}
	};

	typedef bool (* primitive_value_fn)(primitive_value*, const primitive_value*);
}

namespace std
{
	inline ostream& operator<<(ostream& os, const o2::primitive_type& t)
	{
		switch (t)
		{
		case o2::primitive_type::int8:
			return os << "int8";
		case o2::primitive_type::uint8:
			return os << "uint8";
		case o2::primitive_type::int16:
			return os << "int16";
		case o2::primitive_type::uint16:
			return os << "uint16";
		case o2::primitive_type::int32:
			return os << "int32";
		case o2::primitive_type::uint32:
			return os << "uint32";
		case o2::primitive_type::int64:
			return os << "int64";
		case o2::primitive_type::uint64:
			return os << "uint64";
		case o2::primitive_type::float32:
			return os << "float32";
		case o2::primitive_type::float64:
			return os << "float64";
		case o2::primitive_type::bool_:
			return os << "bool_";
		case o2::primitive_type::ptr:
			return os << "ptr";
		default:
			return os << "?";
		}
	}
}