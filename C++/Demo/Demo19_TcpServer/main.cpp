#include "TCPServer.h"
#include <iostream>

int main()
{
	FSNet::TCPServer tcpServer;
	tcpServer.StartUp();
	tcpServer.Open();
	tcpServer.Bind("localhost", 7001);
	tcpServer.Listen(2);
	while (1)
	{
		tcpServer.Accept();
	}

	getchar();

	return 0;
}