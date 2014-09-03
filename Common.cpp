#include "stdafx.h"
#include "Common.h"
#include <windows.h>
#include <stdio.h>
#include <wtypes.h>

namespace Common
{
	std::wstring ansi2unicode(const std::string& ansi)
	{
		if (ansi.empty()) {  
			return std::wstring(L"");  
		}  
		int len = MultiByteToWideChar(CP_ACP, 0, ansi.c_str(), -1, NULL, 0);  
		std::wstring unicode(len + 2, L'\0');  
		len = MultiByteToWideChar(CP_ACP, 0, ansi.c_str(), ansi.size(), &unicode[0], len);  
		return unicode;  
	}

	std::string unicode2ansi(const std::wstring& unicode)
	{
		if (unicode.empty()) {  
			return std::string("");  
		}  
		int len = WideCharToMultiByte(CP_ACP, 0, unicode.c_str(), -1, NULL, 0, NULL, NULL);  
		std::string ansi(len + 1, '\0');  
		WideCharToMultiByte(CP_ACP, 0, unicode.c_str(), unicode.size(), &ansi[0], len, NULL, NULL);  
		return ansi;  
	}

	std::string toString(const char *fmt, ...)
	{
		char buffer[1024] = {0};
		va_list vaList;
		va_start(vaList, fmt);
		vsnprintf_s(buffer, _countof(buffer), _TRUNCATE, fmt, vaList);
		va_end(vaList);
		return buffer;
	}

	std::string getErrString()
	{
		DWORD errCode = GetLastError();
		TCHAR buffer[1024];
		std::string errString;
		if (FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL, errCode, MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US),
			(LPTSTR)&buffer[0],
			sizeof(buffer), NULL) == 0) {
				errString = Common::toString("Cannot get text error describing (%u)", errCode);
		} else {
			errString = Common::toString("%s (%u)", Common::unicode2ansi(buffer).c_str(), errCode);
		}
		return errString;
	}
}
