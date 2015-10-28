#include "PipeClient.h"
#include "Pipe.h"
#include "Common.h"
#include <vector>

PipeClient::PipeClient(const std::string &name)
	: m_pipe(NULL)
{
	HANDLE handle =CreateFile(Common::ansi2unicode(name).c_str(), GENERIC_READ|GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);
	if (INVALID_HANDLE_VALUE == handle) {
		throw IOException("connect pipe failed:" + Common::getErrString());
	}
	m_pipe = new NamedPipe(handle, false);
}

PipeClient::~PipeClient()
{
	delete m_pipe;
}

int PipeClient::writeMessage(int type, int direction, __int64 time, const std::string &localAccount, const std::string &targetAccount, void *msgBody, unsigned int size)
{
	IM_MSG imMsg;
	imMsg.nIMType = type;
	imMsg.nDirection = direction;
	imMsg.uTime = time;
	std::wstring wstrLocalAccount = Common::ansi2unicode(localAccount);
	wcsncpy_s(imMsg.szLocalAccount, wstrLocalAccount.c_str(), min(wstrLocalAccount.size(), MAX_NAME_LEN - 1));
	std::wstring wstrTargetAccount = Common::ansi2unicode(targetAccount);
	wcsncpy_s(imMsg.szTargetAccount, wstrTargetAccount.c_str(), min(wstrTargetAccount.size(), MAX_NAME_LEN - 1));
	imMsg.nMessageLen = size;

	std::vector<char> buffer;
	buffer.resize(sizeof(IM_MSG) + size);
	memcpy(&buffer[0], &imMsg, sizeof(imMsg));
	memcpy(&buffer[0] + sizeof(imMsg), msgBody, size);

	if (m_pipe) {
		return m_pipe->write(&buffer[0], sizeof(imMsg) + size);
	}
	return -1;
}