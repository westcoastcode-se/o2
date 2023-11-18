//
// Part of the o2 Project, under the Apache License v2.0 with o2 Project Exceptions.
// See the LICENSE file in the project root for license terms
//

#pragma once

#include <thread>
#include <future>
#include <atomic>
#include <queue>

namespace o2
{
	/**
	 * \brief a thread-safe channel where you can put results in and out
	 * \tparam T
	 */
	template<typename T>
	class channel
	{
	public:
		channel()
				: _closed(false)
		{
		}

		/**
		 * \brief wait until there's a value and then to pop a value from this channel
		 * \param val the destination where to put the value
		 * \return true if a value is returned
		 */
		bool wait_pop(T* val)
		{
			// wait for the channel to be triggered
			std::unique_lock l(_mutex);
			_condition.wait(l, [this]
			{
				return !_queue.empty() || _closed;
			});

			// is the channel closed?
			if (_closed)
				return false;

			// get the value from the queue
			*val = std::move(_queue.front());
			_queue.pop();
			return true;
		}

		/**
		 * \brief put a value into this channel
		 * \param t
		 * \return true if the value was added to the channel
		 */
		bool put(const T& t)
		{
			if (_closed)
				return false;
			std::lock_guard l(_mutex);
			_queue.push(t);
			_condition.notify_one();
			return true;
		}

		/**
		 * \brief close the channel down and stop allowing more data
		 */
		void close()
		{
			if (_closed)
				return;
			std::lock_guard l(_mutex);
			_closed = true;
			_condition.notify_all();
		}

		/**
		 * \return true if this channel is still opened
		 */
		bool is_open()
		{
			std::lock_guard l(_mutex);
			return !_closed;
		}

		bool empty()
		{
			std::lock_guard l(_mutex);
			return _queue.empty();
		}

	private:
		bool _closed;
		std::mutex _mutex;
		std::condition_variable _condition;
		std::queue<T> _queue;
	};
}