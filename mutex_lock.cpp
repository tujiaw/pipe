#include "stdafx.h"
#include "mutex_lock.h"
#include <windows.h>

struct CMutexLock::Internal
{
	CRITICAL_SECTION mutex;
};

CMutexLock::CMutexLock() 
	: m_internal(new Internal)
{
	InitializeCriticalSection(&m_internal->mutex);
}

CMutexLock::~CMutexLock()
{
	DeleteCriticalSection(&m_internal->mutex);
	delete m_internal;
}

void CMutexLock::lock()
{
	EnterCriticalSection(&m_internal->mutex);
}

void CMutexLock::unlock()
{
	LeaveCriticalSection(&m_internal->mutex);
}

void* CMutexLock::cs()
{
	return &m_internal->mutex;
}

GlobalMutexLock::GlobalMutexLock(const char* name)
{
	m_handle = CreateMutexA(NULL, false, name);
}

GlobalMutexLock::~GlobalMutexLock()
{
	CloseHandle(m_handle);
}

void GlobalMutexLock::lock()
{
	WaitForSingleObject(m_handle, INFINITE);
}

void GlobalMutexLock::unlock()
{
	ReleaseMutex(m_handle);
}


