#include <iostream>
#include <process.h>
#include "ws2tcpip.h"
#include "AtlBase.h"

#pragma comment(lib,"ws2_32.lib")

//STATEMENT
#define SERVER_IP "192.168.7.117"
#define SERVER_PORT 17110

//GLOBAL
WSADATA m_wsaData;
SOCKET m_SocketListen;
SOCKET m_SocketConn;
SOCKADDR_IN m_ServerAddr;
SOCKADDR_IN m_ClientAddr;
HANDLE m_hThread_Accept;
UINT m_iThread_Accept;

UINT WINAPI uiThread_Recv(LPVOID lpParam)
{
	//while (pThis->m_SocketConn != INVALID_SOCKET)
	//{
	//	int nRecvBytes = SOCKET_ERROR;
	//	char cRecvBuf[33] = "";
	//	nRecvBytes = recv(pThis->m_SocketConn, cRecvBuf, 32, 0);
	//	if (nRecvBytes <= 0) break;
	//	cRecvBuf[nRecvBytes] = '\0';
	//	printf("[RECV ]: (%2d) %s\n", nRecvBytes, cRecvBuf);
	//	pThis->MsgProc(cRecvBuf);
	//}

	//pThis->CloseSocketConn();
	//printf("[INFO ] > uiThread_Recv > connect closed. Terminated.\n");
	return 0;
}

UINT WINAPI uiThread_Accept(LPVOID lpParam)
{
	int iClientAddr_length = sizeof(sockaddr_in);
	memset(&(m_ClientAddr), 0, iClientAddr_length);

	if ((m_SocketConn = accept(m_SocketListen, (sockaddr *)&(m_ClientAddr), &iClientAddr_length)) == INVALID_SOCKET)
	{
		printf("[ERR] uiThread_Accept > SocketConn accept failed !!!\n");
		closesocket(m_SocketConn);
		WSACleanup();
		return 1;
	}
	printf("[INF] uiThread_Accept > SocketConn accept success !!!\n");
	/*ThreadRecvBegin();*/

	// Close listen socket when accept succeed
	closesocket(m_SocketListen);

	printf("[INF] uiThread_Accept > Thread_Accept Terminated!\n");
	return 0;
}

/**
* Init Server & Start accept thread
* Return   :   [-1]: failed   [0]: success
*/
int InitServer()
{
	// Init winsock data
	if (WSAStartup(MAKEWORD(2, 2), &m_wsaData) != 0)
	{
		printf("[ERR] InitServer > WSAStartup failed !!!\n");
		return -1;
	}
	// Create listen socket: Head & Data
	if ((m_SocketListen = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == INVALID_SOCKET)
	{
		printf("[ERR] InitServer > Create SocketListen failed !!!\n");
		WSACleanup();
		return -1;
	}
	// Set server ip & port
	memset(&m_ServerAddr, 0, sizeof(SOCKADDR_IN));
	m_ServerAddr.sin_family = AF_INET;
	m_ServerAddr.sin_port = htons(SERVER_PORT);
	inet_pton(AF_INET, SERVER_IP, &m_ServerAddr.sin_addr);
	//m_Server_Addr.sin_addr.S_un.S_addr = inet_addr("192.168.5.48"); InetPton("192.168.5.48");
	// Bind socket
	if (bind(m_SocketListen, (sockaddr *)&m_ServerAddr, sizeof(sockaddr_in)) != 0)
	{
		printf("[ERR] InitServer > Bind SocketListen failed !!!\n");
		closesocket(m_SocketListen);
		WSACleanup();
		return -1;
	}
	// Listen start
	if (listen(m_SocketListen, 2) != 0)
	{
		printf("[ERR] InitServer > Start listen failed !!!\n");
		closesocket(m_SocketListen);
		WSACleanup();
		return -1;
	}

	// Create accept thread
	m_hThread_Accept = (HANDLE)_beginthreadex(NULL, 0, &uiThread_Accept, NULL, 0, &m_iThread_Accept);
	printf("[INF] InitServer > Accept thread started. %d\n", m_iThread_Accept);

	return 0;
}

int main()
{
	InitServer();

	while (true)
	{
		if (m_SocketConn != 0)
		{
			int nRecvBytes = SOCKET_ERROR;
			char cRecvBuf[33] = "";
			nRecvBytes = recv(m_SocketConn, cRecvBuf, 32, 0);
			if (nRecvBytes <= 0) break;
			cRecvBuf[nRecvBytes] = '\0';
			printf("[RECV ]: (%2d) %s\n", nRecvBytes, cRecvBuf);

			WCHAR wsz[64];
			swprintf_s(wsz, L"%S", cRecvBuf);
			::ShellExecute(NULL, L"open", wsz, NULL, L".", SW_NORMAL);
			/*MsgProc(cRecvBuf);*/
		}
		else
			Sleep(1000);
	}

	if (m_SocketConn != NULL)
	{
		closesocket(m_SocketConn); // Close SocketConn when disconnected.
		WSACleanup();
		m_SocketConn = NULL;
	}
	printf("[INF] > main > connect closed. Terminated.\n");
	system("pause");
	return 0;
}