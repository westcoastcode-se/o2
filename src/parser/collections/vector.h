//
// Part of the o2 Project, under the Apache License v2.0 with o2 Project Exceptions.
// See the LICENSE file in the project root for license terms
//

#pragma once

#include <cassert>
#include <memory>

namespace o2
{
	template<typename T>
	class array_view;

	/**
	* \brief vector that holds a mutable number of items in an array-like structure
	* 
	* It essentially solves the same problem as the standard std::vector, but it has a 
	* array_view
	**/
	template<typename T, int Resize = 4>
	class vector
	{
	public:
		vector()
				: _capacity(0), _memory(nullptr), _size(0)
		{
		}

		/**
		 * \brief copy constructor
		 * \param rhs
		 */
		vector(const vector<T, Resize>& rhs)
				: _capacity{ rhs._size }, _memory((T*)malloc(sizeof(T) * _capacity)), _size{ rhs._size }
		{
			for (auto i = 0; i < _size; ++i)
				_memory[i] = rhs._memory[i];
		}

		/**
		 * \brief move constructor
		 * \param source
		 */
		vector(vector<T, Resize>&& source) noexcept
				: _capacity{ source._capacity }, _memory{ source._memory }, _size{ source._size }
		{
			source._capacity = 0;
			source._memory = nullptr;
			source._size = 0;
		}

		explicit vector(int initialSize)
				: _capacity(initialSize), _memory((T*)malloc(sizeof(T) * _capacity)), _size(initialSize)
		{
			assert(_memory != nullptr);
		}

		template<typename ...Args>
		vector(Args... args)
				: _capacity(sizeof...(Args)), _memory((T*)malloc(sizeof(T) * _capacity)), _size(_capacity)
		{
			_Varset(0, args...);
		}

		vector(std::initializer_list<T> ii)
				: _capacity((int)ii.size()), _memory((T*)malloc(sizeof(T) * _capacity)), _size(_capacity)
		{
			T* ptr = _memory;
			for (auto i: ii)
				*ptr++ = i;
		}

		~vector()
		{
			if (_memory)
			{
				free(_memory);
				_memory = nullptr;
			}
		}

		/**
		 * \return the number of items in this array
		 */
		int size() const
		{
			return _size;
		}

		/**
		 * \return true if this vector is empty
		 */
		bool empty() const
		{
			return _size == 0;
		}

		/**
		 * \brief add a new item to this vector
		 * \param value the item we want to add
		 */
		void add(T value)
		{
			if constexpr (std::is_pointer<T>())
				assert(value != nullptr);
			// Resize if neccessary
			if (_size == _capacity)
				_Resize();
			_memory[_size++] = value;
		}

		/**
		 * \brief add a value and make sure that no other instances of this value exists
		 * \param value the value
		 */
		void add_unique(T value)
		{
			if (find(value) == -1)
				add(value);
		}

		/**
		 * \brief put an item at a specific location in the array. The operator[] overload can be used for this as well
		 * \param value the item we want to add
		 * \param idx the location in which the item should be added to
		 */
		void put(T value, int idx)
		{
			assert(_capacity < idx);
			_memory[idx] = value;
		}

		/**
		 * \brief remove the item at the specific index. This will squash the array
		 * \param idx the location we want to remove
		 * \return the value we just removed
		 */
		T remove_at(int idx)
		{
			assert(_capacity > idx);
			assert(_size > idx);

			// Move all items backwards one step
			T removed = _memory[idx];
			for (int i = idx + 1; i < _size; ++i)
			{
				_memory[i - 1] = _memory[i];
			}
			_size--;
			return removed;
		}

 		/**
		 * \brief remove the supplied value
		 * \param value the value
		 */
		void remove(T value)
		{
			const auto idx = find(value);
			if (idx == -1)
				return;
			remove_at(idx);
		}

		/**
		 * \brief search for the index that holds the supplied value
		 * \param value the value we are searching for
		 * \return an index. Returns -1 if no item was found
		 */
		int find(T value)
		{
			for (int i = 0; i < _size; ++i)
				if (value == _memory[i])
					return i;
			return -1;
		}

		/**
		 * \brief insert the supplied array at the supplied index. All items after the supplied index
		 *        are automatically moved forward. Returns the number of new items added to the list
		 *
		 * \param arr the array with items we want to add
		 * \param index the index location where we start putting the first item in the supplied array
		 * \return the number of items added to this vector
		 */
		int insert(array_view<T> arr, int index)
		{
			assert(index >= 0);
			if (arr.empty()) return 0;
			if (index >= _size)
				add(arr);
			else
			{
				// Resize the memory block so that we can fit all items
				if (_size + arr.size() > _capacity)
					_Resize(_size + arr.size() + Resize);
				_size += arr.size();

				// Move forward items from the back towards the front
				const T* src = &_memory[_size - arr.size() - 1];
				T* dest = &_memory[_size - 1];
				const T* end = &_memory[index + arr.size() - 1];
				for (; dest != end; --src, --dest)
					*dest = *src;

				// Copy data from the source array into the destination
				const int size = arr.size();
				for (int i = 0; i < size; ++i)
					_memory[i + index] = arr[i];
			}
			return arr.size();
		}

		/**
		 * \brief add all items in the supplied array
		 * \param arr the array of items we want to add
		 */
		void add(array_view<T> arr)
		{
			for (auto a: arr)
				add(a);
		}

		/**
		 * \brief clear this array.
		 * \param resize set to true if you also want to resize the memory array
		 */
		void clear(bool resize = false)
		{
			assert(resize == false &&
				   "resize feature is not implemented yet");
			_size = 0;
		}

		T* ptr()
		{
			return _memory;
		}

		const T* ptr() const
		{
			return _memory;
		}

	public:
		T& operator[](int idx)
		{
			return _memory[idx];
		}

		const T& operator[](int idx) const
		{
			return _memory[idx];
		}

		/**
		 * \brief move operator
		 * \param rhs
		 * \return
		 */
		vector<T, Resize>& operator=(vector<T, Resize>&& rhs) noexcept
		{
			if (_memory == rhs._memory)
				return *this;
			if (_memory)
			{
				free(_memory);
				_memory = nullptr;
			}
			_size = rhs._size;
			_capacity = rhs._capacity;
			_memory = rhs._memory;
			rhs._capacity = 0;
			rhs._size = 0;
			rhs._memory = nullptr;
			return *this;
		}

		/**
		 * \brief copy operator
		 * \param rhs
		 * \return a reference to this vector
		 */
		vector<T, Resize>& operator=(const vector<T, Resize>& rhs)
		{
			if (_memory == rhs._memory)
				return *this;
			if (_memory)
			{
				free(_memory);
				_memory = nullptr;
			}
			_size = rhs._size;
			_capacity = _size;
			_memory = (T*)malloc(sizeof(T) * _capacity);
			for (int i = 0; i < _size; ++i)
				_memory[i] = rhs._memory[i];
			return *this;
		}

		/**
		 * \brief assignment operator from a read-only array
		 * \param rhs
		 * \return
		 */
		vector<T, Resize>& operator=(const array_view<T>& rhs);

		typedef T* iterator;
		typedef const T* const_iterator;

		iterator begin()
		{
			return _memory;
		}

		const_iterator begin() const
		{
			return _memory;
		}

		iterator end()
		{
			return &_memory[_size];
		}

		const_iterator end() const
		{
			return &_memory[_size];
		}

	private:
		void _Varset(int index, T first)
		{
			_memory[index] = first;
		}

		template<typename ...Args>
		void _Varset(int index, T first, Args... args)
		{
			_memory[index] = first;
			_Varset(index + 1, args...);
		}

		void _Resize()
		{
			_Resize(Resize);
		}

		void _Resize(int minCount)
		{
			_capacity += minCount;
			if (_memory == nullptr)
				_memory = static_cast<T*>(malloc(sizeof(T) * _capacity));
			else
			{
				auto newMemory = static_cast<T*>(realloc(_memory, sizeof(T) * _capacity));
				assert(newMemory);
				_memory = newMemory;
			}
		}

		int _capacity;
		T* _memory;
		int _size;
	};

	/**
	 * \brief class which holds an array of a fixed size. Is compatible with the array_view interface
	 * \tparam T the type contained by this array
	 * \tparam Count the number of elements
	 */
	template<typename T, int Count>
	class array
	{
	public:
		array()
		{
		}

		template<typename ...Args>
		array(Args... args)
		{
			_Varset(0, args...);
		}

		/**
		 * \return the size of the array
		 */
		int size() const
		{
			return Count;
		}

		/**
		 * \brief set the supplied value at the supplied index
		 * \param value
		 * \param idx
		 */
		void set(T value, int idx)
		{
			assert(idx < Count && idx > 0);
			_memory[idx] = value;
		}

		T* ptr()
		{
			return _memory;
		}

		const T* ptr() const
		{
			return _memory;
		}

	public:
		T& operator[](int idx)
		{
			return _memory[idx];
		}

		const T& operator[](int idx) const
		{
			return _memory[idx];
		}

	public:
		typedef T* iterator;
		typedef const T* const_iterator;

		iterator begin()
		{
			return _memory;
		}

		const_iterator begin() const
		{
			return _memory;
		}

		iterator end()
		{
			return &_memory[Count];
		}

		const_iterator end() const
		{
			return &_memory[Count];
		}

	private:
		void _Varset(int index, T first)
		{
			_memory[index] = first;
		}

		template<typename ...Args>
		void _Varset(int index, T first, Args... args)
		{
			_memory[index] = first;
			_Varset(index + 1, args...);
		}

	private:
		T _memory[Count];
	};

	/**
	 * \brief a view of the array object but exposes the implementations as interfaces
	 * \tparam T the type
	 */
	template<typename T>
	class array_view
	{
	private:
		T* _memory;
		int _size;

	public:
		array_view()
				: _memory(nullptr), _size(0)
		{
		}

		template<class Class, int Resize>
		array_view(const vector<Class, Resize>& v)
				: _memory(get_memory(v)), _size(get_size(v))
		{
		}

		template<class Class, int Size>
		array_view(const array<Class, Size>& v)
				: _memory(get_memory(v)), _size(get_size(v))
		{
		}

		template<int Resize>
		array_view(const vector<T, Resize>& rhs)
				: _memory(get_memory(rhs)), _size(get_size(rhs))
		{
		}

		/**
		 * \return the number of items available in the array
		 */
		[[nodiscard]] int size() const
		{
			return _size;
		}

		/**
		 * \return a boolean indicating if this array is empty or not
		 */
		[[nodiscard]] bool empty() const
		{
			return _size == 0;
		}

		/**
		 * \return the underlying memory
		 */
		T* ptr()
		{
			return _memory;
		}

		/**
		 * \return the underlying memory
		 */
		const T* ptr() const
		{
			return _memory;
		}

		template<typename U>
		int index_of(U value) const
		{
			for (int i = 0; i < _size; ++i)
				if (value == _memory[i])
					return i;
			return -1;
		}

	private:
		template<class Class, int Resize>
		static int get_size(const vector<Class, Resize>& v)
		{
			using namespace std;
			if constexpr (is_pointer<Class>())
			{
				if constexpr (is_base_of<typename remove_pointer<T>::type, typename remove_pointer<Class>::type>())
				{
					return v.size();
				}
			}
			else
			{
				return v.size();
			}
		}

		template<class Class, int Resize>
		static T* get_memory(const vector<Class, Resize>& v)
		{
			using namespace std;
			if constexpr (is_pointer<Class>())
			{
				if constexpr (is_base_of<typename remove_pointer<T>::type, typename remove_pointer<Class>::type>())
				{
					Class* memory = const_cast<vector<Class, Resize>&>(v).ptr();
					return reinterpret_cast<T*>(memory);
				}
			}
			else
			{
				return const_cast<vector<Class, Resize>&>(v).ptr();
			}
		}

		template<class Class, int Size>
		static int get_size(const array<Class, Size>& v)
		{
			using namespace std;
			if constexpr (is_pointer<Class>())
			{
				if constexpr (is_base_of<typename remove_pointer<T>::type, typename remove_pointer<Class>::type>())
				{
					return v.size();
				}
			}
			else
			{
				return v.size();
			}
		}

		template<class Class, int Size>
		static T* get_memory(const array<Class, Size>& v)
		{
			using namespace std;
			if constexpr (is_pointer<Class>())
			{
				if constexpr (is_base_of<typename remove_pointer<T>::type, typename remove_pointer<Class>::type>())
				{
					Class* memory = const_cast<array<Class, Size>&>(v).ptr();
					return reinterpret_cast<T*>(memory);
				}
			}
			else
			{
				return const_cast<array<Class, Size>&>(v).ptr();
			}
		}

	public:
		T operator[](int idx)
		{
			return _memory[idx];
		}

		const T operator[](int idx) const
		{
			return _memory[idx];
		}

		/**
		 * \brief assignment operator
		 * \param rhs
		 * \return
		 */
		array_view<T>& operator=(const array_view<T>& rhs)
		{
			_memory = rhs._memory;
			_size = rhs._size;
			return *this;
		}

	public:
		typedef T* iterator;
		typedef const T* const_iterator;

		iterator begin()
		{
			return _memory;
		}

		const_iterator begin() const
		{
			return _memory;
		}

		iterator end()
		{
			return &_memory[_size];
		}

		const_iterator end() const
		{
			return &_memory[_size];
		}
	};

	template<typename T, int Resize>
	vector<T, Resize>& vector<T, Resize>::operator=(const array_view<T>& rhs)
	{
		if (_memory == rhs.ptr())
			return *this;
		if (_memory)
		{
			free(_memory);
			_memory = nullptr;
		}
		_size = rhs.size();
		_capacity = _size;
		_memory = static_cast<T*>(malloc(sizeof(T) * _capacity));
		auto memory = rhs.ptr();
		for (int i = 0; i < _size; ++i)
			_memory[i] = memory[i];
		return *this;
	}
}