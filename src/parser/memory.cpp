//
// Part of the o2 Project, under the Apache License v2.0 with o2 Project Exceptions.
// See the LICENSE file in the project root for license terms
//

#include "memory.h"
#include <cstdlib>
#include <iostream>

using namespace o2;

namespace
{
	const char* REMOVED = "REMOVED";

	struct singleton
	{
		memory_tracker_marker* first;
		memory_tracker_marker* last;
		std::size_t total_bytes;
		bool enabled;
		bool verbose;
	};
	singleton _singleton;
}

void o2::memory_tracker::begin()
{
#if defined(O2_MEMORY_TRACKING)
	_singleton.first = nullptr;
	_singleton.last = nullptr;
	_singleton.total_bytes = 0;
	_singleton.enabled = true;
	_singleton.verbose = false;
#endif
}

bool o2::memory_tracker::end()
{
#if defined(O2_MEMORY_TRACKING)
	if (_singleton.total_bytes > 0)
	{
		auto m = _singleton.first;
		printf("\n===================\n");
		printf("Total allocated memory not released is: %d\n", (int)_singleton.total_bytes);
		while (m)
		{
			printf("%s@%d - [%p] %d bytes\n", m->filename, m->line, (void*)m, (int)m->size);
			m = m->tail;
		}
		printf("===================\n");
		_singleton.first = nullptr;
		_singleton.last = nullptr;
		_singleton.total_bytes = 0;
		_singleton.enabled = true;
		_singleton.verbose = false;
		return false;
	}
	return true;
#else
	return true;
#endif
}

void memory_tracker::verbose()
{
#if defined(O2_MEMORY_TRACKING)
	_singleton.verbose = true;
#endif
}

void o2::memory_tracker::add(o2::memory_tracker_marker* marker)
{
	if (!_singleton.enabled)
		return;

	if (_singleton.verbose)
		printf("[%p] size=+%d\n", (void*)marker, (int)marker->size);

	if (_singleton.first == nullptr)
		_singleton.first = _singleton.last = marker;
	else
	{
		marker->head = _singleton.last;
		_singleton.last->tail = marker;
		_singleton.last = marker;
	}
	_singleton.total_bytes += marker->size;
}

void o2::memory_tracker::remove(o2::memory_tracker_marker* marker)
{
	if (!_singleton.enabled)
		return;

	if (_singleton.verbose)
		printf("[%p] size=-%d\n", (void*)marker, (int)marker->size);

	if (_singleton.first == marker)
		_singleton.first = _singleton.first->tail;
	if (_singleton.last == marker)
		_singleton.last = _singleton.last->head;
	if (marker->head)
		marker->head->tail = marker->tail;
	if (marker->tail)
		marker->tail->head = marker->head;
	_singleton.total_bytes -= marker->size;
	marker->filename = REMOVED;
}

void* o2::memory_tracker::alloc_mem(std::size_t size, const char* filename, int line)
{
	char* bytes = (char*)::malloc(size + sizeof(memory_tracker_marker));
	if (bytes == nullptr)
		return nullptr;
	const auto marker = (memory_tracker_marker*)bytes;
	marker->head = marker->tail = nullptr;
	marker->filename = filename;
	marker->line = line;
	marker->size = size;
	add(marker);
	return bytes + sizeof(memory_tracker_marker);
}

void* o2::memory_tracker::realloc_mem(void* ptr, std::size_t new_size, const char* filename, int line)
{
	ptr = (char*)ptr - sizeof(memory_tracker_marker);
	remove(static_cast<memory_tracker_marker*>(ptr));
	const auto new_mem = ::realloc(ptr, new_size + sizeof(memory_tracker_marker));
	if (new_mem == nullptr)
	{
		// re-add the old marker
		add(static_cast<memory_tracker_marker*>(ptr));
		return nullptr;
	}

	const auto marker = (memory_tracker_marker*)new_mem;
	marker->head = marker->tail = nullptr;
	marker->filename = filename;
	marker->line = line;
	marker->size = new_size;
	add(marker);
	return (char*)new_mem + sizeof(memory_tracker_marker);
}

void o2::memory_tracker::free_mem(void* ptr)
{
	ptr = (char*)ptr - sizeof(memory_tracker_marker);
	remove((memory_tracker_marker*)ptr);
	::free(ptr);
}

#if defined(O2_MEMORY_TRACKING)

void* memory_tracked::operator new(std::size_t size, const char* filename, int line) noexcept
{
	const auto tracked = ::malloc(size);
	if (tracked == nullptr)
		return nullptr;
	const auto t = (memory_tracked*)tracked;
	const auto marker = &t->_mem_marker;
	marker->head = marker->tail = nullptr;
	marker->filename = filename;
	marker->line = line;
	marker->size = size;
	memory_tracker::add(marker);
	return tracked;
}

void memory_tracked::operator delete(void* p, const char* filename, int line)
{
	const auto t = (memory_tracked*)p;
	if (t->_mem_marker.size > 0)
		memory_tracker::remove(&t->_mem_marker);
	::free(p);
}

void memory_tracked::operator delete(void* p)
{
	const auto t = (memory_tracked*)p;
	if (t->_mem_marker.size > 0)
		memory_tracker::remove(&t->_mem_marker);
	::free(p);
}

#endif