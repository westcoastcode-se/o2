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
		 * \brief pop a value from this channel
		 * \return the value
		 */
		T pop()
		{
			std::unique_lock l(_mutex);
			_condition.wait(l, [&]()
			{
				return _closed || _queue.size() > 0;
			});
			if (_closed)
				throw std::runtime_error("channel is closed");
			auto ret = std::move(_queue.front());
			_queue.pop();
			_condition.notify_one();
			return ret;
		}

		/**
		 * \brief try to pop a value from t
		 * \param val
		 * \return
		 */
		template<class _Rep, class _Period>
		bool try_pop(T* val, const std::chrono::duration<_Rep, _Period>& time)
		{
			std::unique_lock l(_mutex);
			const bool st = _condition.wait_for(l, time, [&]()
			{
				return _closed || _queue.size() > 0;
			});
			if (!st || _queue.size() == 0)
				return false;
			if (_closed)
				throw std::runtime_error("channel is closed");
			*val = std::move(_queue.front());
			_queue.pop();
			_condition.notify_one();
			return true;

		}

		/**
		 * \brief put a value into this channel
		 * \param t
		 */
		void put(const T& t)
		{
			std::unique_lock l(_mutex);
			_queue.push(t);
			_condition.notify_one();
		}

		/**
		 * \brief close the channel down and stop allowing more data
		 */
		void close()
		{
			_closed = true;
			_condition.notify_one();
		}

		/**
		 * \return true if this channel is still opened
		 */
		bool is_open() const
		{
			return !_closed;
		}

		bool empty()
		{
			std::unique_lock l(_mutex);
			return _queue.empty();
		}

	private:
		std::atomic_bool _closed;
		std::mutex _mutex;
		std::condition_variable _condition;
		std::queue<T> _queue;
	};
}