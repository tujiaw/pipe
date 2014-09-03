#pragma once

#include <windows.h>
#include <string>

class WindowEvent
{
public:
	WindowEvent(const std::string &name = "");
	virtual ~WindowEvent(void);

	void notify();
	void waitForEvent(DWORD milliseconds = INFINITE);
	HANDLE getHandle() const { return event_; }

protected:
	void setAccessToAll(HANDLE objHandle);

private:
	HANDLE event_;
};

