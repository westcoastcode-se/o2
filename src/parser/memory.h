//
// Part of the o2 Project, under the Apache License v2.0 with o2 Project Exceptions.
// See the LICENSE file in the project root for license terms
//

#pragma once

#include <memory>

namespace o2
{
	struct memory_tracker_marker
	{
		const char* filename;
		int line;
		std::size_t size;
		memory_tracker_marker* head;
		memory_tracker_marker* tail;
	};

	/**
	 * \brief keeps track on memory to ensure that we don't have any memory leaks
	 */
	class memory_tracker
	{
	public:
		/**
		 * \brief begin memory tracking
		 */
		static void begin();

		/**
		 * \brief end memory tracking
		 */
		static bool end();

		/**
		 * \brief enable verbose mode
		 */
		static void verbose();

		static void add(memory_tracker_marker* marker);

		static void remove(memory_tracker_marker* marker);

		static void* alloc_mem(std::size_t size, const char* filename, int line);

		static void* realloc_mem(void* ptr, std::size_t new_size, const char* filename, int line);

		static void free_mem(void* ptr);
	};

	/**
	 * \brief helper class used if you want to have memory tracking enabled
	 *
	 * you have to define O2_MEMORY_TRACKING if you want memory tracking to be enabled
	 */
	class memory_tracked
	{
	public:
#if defined(O2_MEMORY_TRACKING)
		virtual ~memory_tracked() = default;
		static void* operator new(std::size_t size, const char* filename, int line) noexcept;
		static void operator delete(void* p, const char* filename, int line);
		static void operator delete(void* p);
		friend class memory_tracker;

	private:
		memory_tracker_marker _mem_marker;
#endif
	};
}

#if defined(O2_MEMORY_TRACKING)
#define arl_new new(__FILE__, __LINE__)
#define arl_malloc(size) o2::memory_tracker::alloc_mem(size, __FILE__, __LINE__)
#define arl_free(ptr) o2::memory_tracker::free_mem(ptr)
#define arl_realloc(ptr, new_size) o2::memory_tracker::realloc_mem(ptr, new_size, __FILE__, __LINE__)
#else
#define arl_new new
#define arl_malloc malloc
#define arl_free free
#define arl_realloc realloc
#endif
