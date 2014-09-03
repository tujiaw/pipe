// pipe.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <iostream>

#include "PipeServer.h"
#include "Pipe.h"

int _tmain(int argc, _TCHAR* argv[])
{
	try
	{
		PipeServer pServer("myTest");
		NamedPipe *pClient = pServer.accept();
		char buffer[1024] = {0};
		pClient->read(buffer, sizeof(buffer));
	}
	catch (IOException &e)
	{
		std::cout << "IOException, " << e.getMessage();
	}
	catch (Exception &e)
	{
		std::cout << "Exception, " << e.getMessage();
	}

	system("pause");
	return 0;
}

