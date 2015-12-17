#ifndef SYNC_QUEUE_H__
#define SYNC_QUEUE_H__
#include <deque>
#include "Common/Logger/Log/include/Base/Condition.h"
#include "Common/Logger/Log/include/Base/Atomic.h"


namespace Logger
{
	template <class T, uint64_t QUEUE_SIZE = 1000>
	class AsyncQueue
	{
	public:
		AsyncQueue() : queue_()
                     , mutex_()
                     , notFullCond_(mutex_)
                     , notEmptyCond_(mutex_)
		{

		}

		void Put(const T& value)
		{
			MutexLockGuard lk(mutex_);
			while (Full())
			{
                notFullCond_.Wait();
			}
			queue_.push_back(value);
			notEmptyCond_.NotifyOne();
		}

		void Put(const T* ptr)
		{
			MutexLockGuard lk(mutex_);
			while (Full())
			{
				notFullCond_.Wait();
			}
			queue_.push_back(ptr);
			notEmptyCond_.NotifyOne();
		}

		T Take()
		{
			MutexLockGuard lk(mutex_);
			while (Empty())
			{
				notEmptyCond_.Wait();
			}
			T frontValue = queue_.front();  
			queue_.pop_front();
			return frontValue;
		}

		uint64_t Size() const
		{
			return queue_.size();
		}


		bool Empty() const
		{
			return queue_.empty();
		}

	private:
		bool Full()	const
		{
			return (queue_.size() >= QUEUE_SIZE);

		}

	private:
        mutable MutexLock mutex_;
		Condition notEmptyCond_;
		Condition notFullCond_;
		std::deque<T> queue_;
	};
}


#endif