#pragma once

class CMutexLock
{
public:
	CMutexLock();
	~CMutexLock();
	void lock();
	void unlock();

	void* cs();

private:
	CMutexLock(const CMutexLock&);
	CMutexLock& operator=(const CMutexLock&);

	struct Internal;
	Internal *m_internal;
};

class CMutexLockGuard
{
public:
	explicit CMutexLockGuard(CMutexLock &mutex)
		: m_mutex(mutex)
	{
		m_mutex.lock();
	}

	~CMutexLockGuard()
	{
		m_mutex.unlock();
	}

private:
	CMutexLockGuard(const CMutexLockGuard&);
	CMutexLockGuard operator=(const CMutexLockGuard&);

	CMutexLock& m_mutex;
};

class GlobalMutexLock
{
public:
	GlobalMutexLock(const char* name);
	~GlobalMutexLock();
	void lock();
	void unlock();

private:
	void* m_handle;
};

class GlobalMutexLockGuard
{
public:
	GlobalMutexLockGuard(GlobalMutexLock &mutex) : m_mutex(mutex)
	{
		m_mutex.lock();
	}

	~GlobalMutexLockGuard()
	{
		m_mutex.unlock();
	}

private:
	GlobalMutexLock &m_mutex;
};