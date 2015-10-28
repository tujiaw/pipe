#pragma once

#include "mutex_lock.h"
#include <WinBase.h>

namespace util {
	bool IsWindowsXP()
	{
		return true;
		static int result = -1;
		if (-1 == result) {
			OSVERSIONINFO osvi;

			ZeroMemory(&osvi, sizeof(OSVERSIONINFO));
			osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);

			GetVersionEx(&osvi);

			bool isXp = osvi.dwMajorVersion  < 6;
			result = isXp ? 1 : 0;

			return result ? true : false;
		}
		return result ? true : false;
	}
}


class Condition
{
public:
	explicit Condition(CMutexLock &mutex)
		: mutex_(mutex)
	{
		if (util::IsWindowsXP()) {
			cond_ = new CondType();
			((CondType*)cond_)->signal = CreateEvent(NULL, false, false, NULL);
			((CondType*)cond_)->broadcast = CreateEvent(NULL, true, false, NULL);
		} else {
			cond_ = new CONDITION_VARIABLE();
			InitializeConditionVariable((PCONDITION_VARIABLE)cond_);
		}
	}

	Condition::~Condition()
	{
		if (util::IsWindowsXP()) {
			CondType *p = (CondType*)cond_;
			CloseHandle(p->signal);
			CloseHandle(p->broadcast);
			delete p;
		} else {
			CONDITION_VARIABLE *p = (CONDITION_VARIABLE*)cond_;
			delete p;
		}
	}

	void wait()
	{
		if (util::IsWindowsXP()) {
			mutex_.unlock();
			HANDLE handles[] = {((CondType*)cond_)->signal, ((CondType*)cond_)->broadcast};
			WaitForMultipleObjects(2, handles, false, INFINITE);
			mutex_.lock();
		} else {
			SleepConditionVariableCS((PCONDITION_VARIABLE)cond_, (CRITICAL_SECTION*)(mutex_.cs()), INFINITE);
		}
	}

	void notify()
	{
		if (util::IsWindowsXP()) {
			SetEvent(((CondType*)cond_)->signal);
		} else {
			WakeConditionVariable((PCONDITION_VARIABLE)cond_);
		}
	}

private:
	Condition(const Condition&);
	Condition& operator=(const Condition&);

private: 
	CMutexLock &mutex_;

	void *cond_;
	typedef struct { HANDLE signal, broadcast; } CondType;
};
