#include "stdafx.h"
#include "PipeServer.h"
#include "Common.h"
#include "Pipe.h"

#define BUFSIZE (512 * 1024)

PipeServer::PipeServer(const std::string &name, DWORD milliseconds)
	: milliseconds_(milliseconds), serverPipe_(INVALID_HANDLE_VALUE)
{
	pipeName_ = Common::toString("\\\\.\\pipe\\%s", name.c_str());
	createServerPipe();
}


PipeServer::~PipeServer(void)
{
	close();
}

NamedPipe* PipeServer::accept()
{
	if (serverPipe_ == INVALID_HANDLE_VALUE) {
		createServerPipe();
	}

	OVERLAPPED overlapped;
	memset(&overlapped, 0, sizeof(OVERLAPPED));
	overlapped.hEvent = winEvent_.getHandle();

	if (ConnectNamedPipe(serverPipe_, &overlapped)) {
		std::string errString = "ConnectNamedPipe failed, " + Common::getErrString();
		throw Exception(errString);
	} else {
		int errCode = GetLastError();
		switch (errCode) {
		case ERROR_PIPE_CONNECTED:
			break;
		case ERROR_IO_PENDING:
			winEvent_.waitForEvent(milliseconds_);
			DWORD cbRet;
			if (!GetOverlappedResult(serverPipe_, &overlapped, &cbRet, FALSE)) {
				std::string errString = "ConnectNamedPipe failed, after GetOverlappedResult, " + Common::getErrString();
				throw Exception(errString);
			}
			break;
		default:
			std::string errString = "ConnectNamedPipe failed, " + Common::getErrString();
			throw Exception(errString);
		}
	}

	NamedPipe *result = new NamedPipe(serverPipe_, true);
	serverPipe_ = INVALID_HANDLE_VALUE;
	return result;
}
void PipeServer::close()
{
	winEvent_.notify();
}

void PipeServer::waitForConnect(DWORD milliseconds)
{

}

void PipeServer::closeConnection()
{

}

void PipeServer::createServerPipe()
{
	serverPipe_ = CreateNamedPipe(Common::ansi2unicode(pipeName_).c_str(),
		PIPE_ACCESS_DUPLEX | FILE_FLAG_OVERLAPPED,
		PIPE_TYPE_BYTE | PIPE_READMODE_BYTE | PIPE_WAIT,
		PIPE_UNLIMITED_INSTANCES,
		BUFSIZE,
		BUFSIZE,
		0,
		0);
	if (serverPipe_ == INVALID_HANDLE_VALUE) {
		std::string errString = "CreateNamedPipe failed, " + Common::getErrString();
		throw Exception(errString);
	}
}