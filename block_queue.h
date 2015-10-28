#pragma  once

#include "windows_condition.h"
#include "mutex_lock.h"
#include <queue>
#include <assert.h>

template<typename T>
class BlockQueue
{
public:
	BlockQueue()
		: mutex_()
		, notEmpty_(mutex_)
	{
	}

	void put(const T &t)
	{
		CMutexLockGuard lock(mutex_);
		queue_.push_back(t);
		notEmpty_.notify();
	}

	T take()
	{
		CMutexLockGuard lock(mutex_);
		while (queue_.empty()) {
			notEmpty_.wait();
		}
		assert(!queue_.empty());
		T front(queue_.front());
		queue_.pop_front();
		return front;
	}

	size_t size() const
	{
		CMutexLockGuard lock(mutex_);
		return queue_.size(); 
	}

private:
	BlockQueue(const BlockQueue&);
	BlockQueue& operator=(const BlockQueue&);

private:
	mutable CMutexLock mutex_;
	Condition notEmpty_;
	std::deque<T> queue_;
};