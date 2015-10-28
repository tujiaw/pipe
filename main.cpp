// pipe.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "Pipe.h"
#include "PipeServer.h"
#include "PipeClient.h"
#include "Common.h"
#include "Exception.h"
#include <iostream>
#include <vector>
#include <process.h>
#include <locale.h>
#include "block_queue.h" // test

#define CLIENT_TEST 0
#define SERVER_TEST 1

#if SERVER_TEST
PipeServer g_server("myTest");
static unsigned int __stdcall startThread(void *object)
{
	if (!object) {
		return 0;
	}

	std::cout << "new client, threadId:" << ::GetCurrentThreadId() << std::endl;
	NamedPipe *client = static_cast<NamedPipe*>(object);

	while (1) {
		IM_MSG *newMsg = NULL;
		bool bExit = false;
		try {
			IM_MSG msg;
			client->read(&msg, sizeof(msg));
			int messageSize = msg.nMessageLen * 2; // 宽字符
			newMsg = (IM_MSG*)new char[sizeof(IM_MSG) + messageSize + 2];
			memset(newMsg, 0, sizeof(IM_MSG) + messageSize + 2);
			*newMsg = msg;

			std::cout << "type:" << newMsg->nIMType << std::endl;
			std::cout << "direction:" << newMsg->nDirection << std::endl;
			std::cout << "time:" << newMsg->uTime << std::endl;
			wprintf(L"local account:%s\n", newMsg->szLocalAccount);
			wprintf(L"target account:%s\n", newMsg->szTargetAccount);
			std::cout << "message len:" << newMsg->nMessageLen << std::endl;
			if (messageSize > 0) {
				client->read(newMsg->szMsg, messageSize);
				wprintf(L"message:%s\n", newMsg->szMsg);
			}
			std::cout << "----------------------------------------------------" << std::endl;
		} catch (IOException &e) {
			std::cout << "IOException, " << e.getMessage() << std::endl;
			bExit = true;
		} catch (Exception &e) {
			std::cout << "Exception, " << e.getMessage() << std::endl;
			bExit = true;
		}

		if (newMsg) {
			delete [] newMsg;
		}

		if (bExit) {
			std::cout << "client exit: threadId:" << ::GetCurrentThreadId() << std::endl;
			break;
		}
	}

	delete client;
	return 0;
}
#endif // SERVER_TEST

int _tmain(int argc, _TCHAR* argv[])
{
	std::locale::global(std::locale(""));
	
#if CLIENT_TEST
	try {
		std::cout << "client start" << std::endl;
		PipeClient client("\\\\.\\pipe\\myTest");

		while (1) {
			std::string line;
			std::getline(std::cin, line);
			if (line == "exit") {
				break;
			}

			std::string str = "fffffdgdfgsadgfdhsdgdfhfdgsdgadsfgad";
			str.resize(1024 * 1024 * 10);
			int result = client.writeMessage(1, 2, 3, "tujiaw", "xinxin.sun", &str[0], str.size());
			std::cout << result << std::endl;
		}
	} catch (IOException &e) {
		std::cout << "IOException, " << e.getMessage();
	} catch (Exception &e) {
		std::cout << "Exception, " << e.getMessage();
	}
#endif // CLIENT_TEST

#if SERVER_TEST
	std::cout << "server start---------------------------" << std::endl;
	while (1) {
		NamedPipe *newConnect = g_server.accept();
		_beginthreadex(NULL, 0, startThread, newConnect, 0, NULL);
	}
#endif // SERVER_TEST

	system("pause");
	return 0;
}

