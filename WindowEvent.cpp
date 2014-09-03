#include "stdafx.h"
#include "WindowEvent.h"
#include "Common.h"
#include "Exception.h"
#include <Aclapi.h>

WindowEvent::WindowEvent(const std::string &name)
{
	event_ = CreateEvent(0, FALSE, FALSE, Common::ansi2unicode(name).c_str());
	if (0 == event_) {
		int errCode = GetLastError();
		std::string errString = Common::toString("Cannot create windows event with error = %d", errCode);
		throw Exception(errString);
	}

	bool needToInit = (GetLastError() != ERROR_ALREADY_EXISTS);
	if (needToInit) {
		setAccessToAll(event_);
	}
}

WindowEvent::~WindowEvent(void)
{
	if (event_) {
		CloseHandle(event_);
	}
}

void WindowEvent::notify()
{
	if (event_) {
		SetEvent(event_);
	}
}
void WindowEvent::waitForEvent(DWORD milliseconds)
{
	if (event_) {
		WaitForSingleObject(event_, milliseconds);
	}
}

void WindowEvent::setAccessToAll(HANDLE objHandle)
{
	DWORD errCode = SetSecurityInfo(objHandle, SE_KERNEL_OBJECT, DACL_SECURITY_INFORMATION, 0, 0, 0, 0);
	if (errCode != ERROR_SUCCESS && errCode != ERROR_NO_SECURITY_ON_OBJECT) {
		std::string errString = Common::toString("Cannot SetSecurityInfo with error = %d", (int)errCode);
		throw Exception(errString);
	}
}