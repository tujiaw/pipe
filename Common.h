#pragma once

#include <string>

namespace Common
{
	std::wstring ansi2unicode(const std::string& ansi);
	std::string unicode2ansi(const std::wstring& unicode);
	std::string toString(const char *fmt, ...);
	std::string getErrString();
}

