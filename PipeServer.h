#pragma once

#include "WindowEvent.h"

class NamedPipe;
class PipeServer
{
public:
	PipeServer(const std::string &name, DWORD milliseconds = INFINITE);
	virtual ~PipeServer(void);

	NamedPipe* accept();
	void close();
	void waitForConnect(DWORD milliseconds = INFINITE);
	virtual void closeConnection();

private:
	void createServerPipe();

private:
	std::string pipeName_;
	WindowEvent winEvent_;
	DWORD milliseconds_;
	HANDLE serverPipe_;
};

