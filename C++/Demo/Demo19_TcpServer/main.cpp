#include "TCPServer.h"
#include <iostream>

int main()
{
	int iRecvBytes = 0;
	char strRecv[32];
	memset(strRecv, 0, sizeof(strRecv));

	FSNet::TCPServer tcpServer;
	tcpServer.StartUp();
	tcpServer.Open();
	tcpServer.Bind("localhost", 7001);
	tcpServer.Listen(2);
	tcpServer.Accept();
	
	while (true)
	{
		iRecvBytes = tcpServer.Receive(strRecv, 31);
		if (iRecvBytes == -1) break;
		strRecv[iRecvBytes] = '\0';
		printf("[RECV](%2d): %s\r\n", iRecvBytes, strRecv);
		if (iRecvBytes == 0) tcpServer.Close();
	}
	
	getchar();

	return 0;
}