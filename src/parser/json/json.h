//
// Part of the o2 Project, under the Apache License v2.0 with o2 Project Exceptions.
// See the LICENSE file in the project root for license terms
//

#pragma once

#include "../strings.h"

namespace o2
{
	/**
	 * \brief a simple json writer
	 */
	struct json
	{
		ostream* _stream;
		int _count;

		enum type
		{
			type_none,
			type_array,
			type_object
		};
		type _type;

		template<typename T>
		void write_value(T val)
		{
			*_stream << val;
		}

		template<>
		void write_value(const string& value)
		{
			*_stream << "\"" << value << "\"";
		}

		template<>
		void write_value(string_view value)
		{
			*_stream << "\"" << value << "\"";
		}

		template<>
		void write_value(const string_literal* value)
		{
			*_stream << "\"" << value << "\"";
		}

	public:
		explicit json(ostream* stream)
				: json(stream, type_none)
		{
		}

		json(ostream* stream, type root_type)
				: _stream(stream), _count(), _type(root_type)
		{
			switch (_type)
			{
			case type_array:
				*_stream << "[";
				break;
			case type_object:
				*_stream << "{";
				break;
			case type_none:
				break;
			}
		}

		json(const json& rhs, type t)
				: _stream(rhs._stream), _count(), _type(t)
		{
			switch (_type)
			{
			case type_array:
				*_stream << "[";
				break;
			case type_object:
				*_stream << "{";
				break;
			case type_none:
				break;
			}
		}

		json(json&& rhs) noexcept
				: _stream(rhs._stream), _count(rhs._count), _type(rhs._type)
		{
		}

		~json()
		{
			switch (_type)
			{
			case type_array:
				*_stream << "]";
				break;
			case type_object:
				*_stream << "}";
				break;
			case type_none:
				break;
			}
		}

		template<typename T>
		struct pair
		{
			string_view key;
			T val;
		};

		template<typename T>
		struct value
		{
			T val;
		};

		struct array
		{
			string_view key;
		};

		struct object
		{
			string_view key;
		};

		/**
		 * \brief write a key-value pair
		 * \tparam T the value type
		 * \param v the value itself
		 */
		template<typename T>
		json& write(const pair<T>& v)
		{
			if (_count++ > 0)
				*_stream << ',';
			*_stream << "\"" << v.key << "\":";
			write_value<T>(v.val);
			return *this;
		}

		/**
		 * \brief write a value
		 * \tparam T the value type
		 * \param v the value itself
		 */
		template<typename T>
		json& write(const value<T>& v)
		{
			if (_count++ > 0)
				*_stream << ',';
			write_value<T>(v.val);
			return *this;
		}

		/**
		 * \brief write an object
		 * \param v the object
		 * \return a new json object that we can use to write the value content
		 */
		json write(const object& v)
		{
			if (_count++ > 0)
				*_stream << ',';
			if (!v.key.empty())
				*_stream << "\"" << v.key << "\":";
			return { *this, type_object };
		}

		/**
		 * \brief write an array
		 * \param v the object
		 * \return a new json object that we can use to write the array content
		 */
		json write(const array& v)
		{
			if (_count++ > 0)
				*_stream << ',';
			if (!v.key.empty())
				*_stream << "\"" << v.key << "\":";
			return { *this, type_array };
		}

		json& operator=(json&& rhs) noexcept
		{
			_stream = rhs._stream;
			_count = rhs._count;
			_type = rhs._type;
		}
	};
}