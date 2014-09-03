#include "stdafx.h"
#include "Exception.h"


Exception::Exception(void)
	: std::exception()
{
}

Exception::Exception(const std::string &message)
	: message_(message), std::exception(message.c_str())
{

}

Exception::~Exception(void)
{
}

std::string Exception::getMessage() const
{
	return message_;
}

///////////////////////////////////////////////////////
IOException::IOException()
	: Exception()
{
}

IOException::IOException(const std::string &message)
	: Exception(message) 
{
}

IOException::~IOException()
{}