#include "stdafx.h"
#include "Pipe.h"
#include "Exception.h"
#include "Common.h"
#include <winnt.h>

#define MAX_PORTION_SIZE (512 * 1024)

size_t Pipe::readByHandle(void *buffer, size_t len, HANDLE pipeHandle)
{
	DWORD result = 0;
	OVERLAPPED overlapped;
	memset(&overlapped, 0, sizeof(OVERLAPPED));
	overlapped.hEvent = readEvent_.getHandle();

	bool success;
	DWORD length = (DWORD)len;
	_ASSERT(length == len);
	checkPipeHandle(pipeHandle);
	success = (ReadFile(pipeHandle, buffer, length, &result, &overlapped) != 0);
	if (!success) {
		DWORD errCode = GetLastError();
		if (errCode == ERROR_IO_PENDING) {
			readEvent_.waitForEvent();
			DWORD cbRet = 0;
			checkPipeHandle(pipeHandle);
			if (GetOverlappedResult(pipeHandle, &overlapped, &cbRet, FALSE) && cbRet != 0) {
				result = cbRet;
			} else {
				std::string errString = "the pipe's read failed after GetOverlappedResult, " + Common::getErrString();
				throw IOException(errString);
			}
		} else {
			std::string errString = "the pipe's read failed after ReadFile, " + Common::getErrString();
			throw IOException(errString);
		}
	}

	if (0 == result) {
		throw IOException("Unknown pipe error");
	}
	return result;
}

size_t Pipe::writeByHandle(const void *buffer, size_t len, HANDLE pipeHandle)
{
	DWORD result;
	OVERLAPPED overlapped;
	memset(&overlapped, 0, sizeof(OVERLAPPED));
	overlapped.hEvent = writeEvent_.getHandle();

	bool success;
	checkPipeHandle(pipeHandle);
	DWORD length = (DWORD)len;
	_ASSERT(length == len);
	success = (WriteFile(pipeHandle, buffer, length, &result, &overlapped) != 0);
	if (!success) {
		int errCode = GetLastError();
		if (errCode == ERROR_IO_PENDING) {
			writeEvent_.waitForEvent();
			DWORD cbRet;
			checkPipeHandle(pipeHandle);
			if (GetOverlappedResult(pipeHandle, &overlapped, &cbRet, FALSE) || cbRet == 0) {
				result = cbRet;
			} else {
				std::string errString = "the pipe's write failed after GetOverlappedResult, " + Common::getErrString();
				throw IOException(errString);
			}
		} else {
			std::string errString = "the pipe's write failed after WriteFile, " + Common::getErrString();
			throw IOException(errString);
		}
	}

	if (0 == result) {
		throw IOException("Unknown pip error");
	}
	return result;
}

void Pipe::checkPipeHandle(HANDLE pipeHandle)
{
	if (pipeHandle == INVALID_HANDLE_VALUE) {
		throw IOException("Invalid pipe handle");
	}
}

////////////////////////////////////////////////////////////////
NamedPipe::NamedPipe(HANDLE hPipe, bool isServer)
	: pipe_(hPipe), isServer_(isServer)
{
}

NamedPipe::~NamedPipe()
{
	try {
		close();
	} catch (...) {
	}
}

void NamedPipe::close()
{
	if (isServer_) {
		if (DisconnectNamedPipe(pipe_) == 0) {
			std::string errString = "DisconnectNamedPipe failed, " + Common::getErrString();
			throw Exception(errString);
		}
	}

	if (pipe_ != INVALID_HANDLE_VALUE) {
		CloseHandle(pipe_);
		pipe_ = INVALID_HANDLE_VALUE;
	}

	readEvent_.notify();
	writeEvent_.notify();
}

size_t NamedPipe::read(void *buffer, size_t len)
{
	return readByHandle(buffer, len, pipe_);
}

size_t NamedPipe::write(const void *buffer, size_t len)
{
	return writeByHandle(buffer, len, pipe_);
}

HANDLE NamedPipe::getHandle() const
{
	return pipe_;
}

void NamedPipe::checkPipeHandle(HANDLE pipeHandle)
{
	if (pipe_ == INVALID_HANDLE_VALUE) {
		throw IOException("Invalid pipe handle");
	}
}