#pragma once

#include "WindowEvent.h"
#include "Exception.h"

class Pipe
{
public:
	size_t readByHandle(void *buffer, size_t len, HANDLE pipeHandle);
	size_t writeByHandle(const void *buffer, size_t len, HANDLE pipeHandle);

private:
	void checkPipeHandle(HANDLE pipeHandle);

private:
	WindowEvent readEvent_;
	WindowEvent writeEvent_;
};

class NamedPipe : public Pipe
{
public:
	NamedPipe(HANDLE hPipe, bool isServer);
	virtual ~NamedPipe();

	void close();
	size_t read(void *buffer, size_t len);
	size_t write(const void *buffer, size_t len);
	HANDLE getHandle() const;

private:
	void checkPipeHandle(HANDLE pipeHandle);

private:
	HANDLE pipe_;
	std::string pipeName_;
	WindowEvent readEvent_;
	WindowEvent writeEvent_;
	bool isServer_;
};