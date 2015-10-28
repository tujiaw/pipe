#pragma once

#include <string>

class NamedPipe;
class PipeClient
{
public:
	PipeClient(const std::string &name);
	~PipeClient();

	int writeMessage(int type, int direction, __int64 time, const std::string &localAccount, const std::string &targetAccount, void *msgBody, unsigned int size);

private:
	NamedPipe *m_pipe;
};