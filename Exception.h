#pragma once

#include <string>

class Exception : public std::exception
{
public:
	Exception(void);
	Exception(const std::string &message);
	virtual ~Exception(void);

	std::string getMessage() const;

private:
	std::string message_;
};

class IOException : public Exception
{
public:
	IOException();
	IOException(const std::string &message);
	virtual ~IOException();
};