#include "TCPServer.h"
#include <iostream>
#include <ws2tcpip.h>

#pragma comment (lib, "Ws2_32.lib")

using namespace FSNet;

/**************************************************************************************************
 * TCPServer::TCPServer -- The constructor for TCPServer
 * 
 *     Initialize m_socketAddr and m_clientAddr as INVALID_SOCKET.
 *================================================================================================*/
TCPServer::TCPServer()
{
	m_socket = INVALID_SOCKET;
	memset(&m_socketAddr, 0, sizeof(SOCKADDR_IN));
	m_client = INVALID_SOCKET;
	memset(&m_clientAddr, 0, sizeof(SOCKADDR_IN));
}

/**************************************************************************************************
 * TCPServer::~TCPServer -- The Destructor for TCPServer
 *
 *     If socket still open then close it. Initialize m_socketAddr and m_clientAddr as 
 *     INVALID_SOCKET. Cleanup the windows socket.
 *================================================================================================*/
TCPServer::~TCPServer()
{
	if (m_socket != INVALID_SOCKET)
	{
		closesocket(m_socket);
		m_socket = INVALID_SOCKET;
	}
	WSACleanup();
}

/**************************************************************************************************
 * TCPServer::StartUp -- StartUp Windows Sockets
 *
 *     Call WSAStartup() to Startup Windows Sockets.
 *================================================================================================*/
bool TCPServer::StartUp()
{
	if (WSAStartup(MAKEWORD(2, 2), &m_wsaData) != NO_ERROR)
	{
		printf("[ERR] TCPServer > StartUp > WSAStartup failed !!! Error: %ld\r\n", WSAGetLastError());
		return false;
	}
	printf("[INF] TCPServer > StartUp > OK !!! %s %s\r\n", m_wsaData.szDescription, m_wsaData.szSystemStatus);
	return true;
}

/**************************************************************************************************
 * TCPServer::Open -- Setup and Open tcp socket
 *
 *     Setup socket protocol to TCP and open socket. Using Blocking mode.
 *================================================================================================*/
bool TCPServer::Open()
{
	m_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (m_socket == INVALID_SOCKET)
	{
		printf("[ERR] TCPServer > Open > m_socket == INVALID_SOCKET\r\n");
		return false;
	}
	printf("[INF] TCPServer > Open > m_socket == %d\r\n", int(m_socket));
	return true;

	/* nonblocking mode*/
	/*
	int iMode = 1;
	int iRst = ioctlsocket(m_socket, FIONBIO, (u_long*)&iMode);
	if (iRst == SOCKET_ERROR)
		printf("ioctlsocket failed!\r\n");
	*/
}

/**************************************************************************************************
 * TCPServer::Bind -- Bind the socket
 *
 *     Bind socket to custom ip:port. Initialize m_socketAddr.
 *
 * INPUT: ip -- The ip address of server.
 *        port -- The number of server listen port.
 *================================================================================================*/
bool TCPServer::Bind(const char* ip, int port)
{
	// Set server ip & port
	m_socketAddr.sin_family = AF_INET;
	m_socketAddr.sin_port = htons(port);
	inet_pton(AF_INET, ip, &m_socketAddr.sin_addr);
	if (bind(m_socket, (sockaddr*)&m_socketAddr, sizeof(m_socketAddr)) == SOCKET_ERROR)
	{
		printf("[ERR] TCPServer > Bind > bind error code: %ld\r\n", WSAGetLastError());
		return false;
	}
	printf("[INF] TCPServer > Bind > bind OK\r\n");
	return true;
}

/**************************************************************************************************
 * TCPServer::Listen -- Listen the socket
 *
 *     Socket run as listen mode.
 *
 * INPUT: backlog -- log level
 *================================================================================================*/
bool TCPServer::Listen(int backlog)
{
	if (listen(m_socket, backlog) == SOCKET_ERROR)
	{
		printf("[ERR] TCPServer > Listen > listen error code: %ld\r\n", WSAGetLastError());
		return false;
	}
	printf("[INF] TCPServer > Listen > listen OK\r\n");
	return true;
}

/**************************************************************************************************
 * TCPServer::Accept -- Accept connect of client
 *
 *     Accept client connection. Store client information in m_clientAddr struct and store 
 *     socket in m_client.
 *================================================================================================*/
bool TCPServer::Accept()
{
	int iClientAddr_length = sizeof(sockaddr_in);
	memset(&(m_clientAddr), 0, iClientAddr_length);

	if ((m_client = accept(m_socket, (sockaddr *)&(m_clientAddr), &iClientAddr_length)) == INVALID_SOCKET)
	{
		printf("[ERR] TCPServer > Accept > client accept failed !!!\r\n");
		return false;
	}
	printf("[INF] TCPServer > Accept > client accept success !!!\r\n");
	/*ThreadRecvBegin();*/
	return true;
}

/**************************************************************************************************
 * TCPServer::Receive -- Receive data from client.
 *
 *     Receive data from client, return length of received data.
 *
 * INTPUT: cBuff -- The pointer to store received data.
 *         iMaxRecv -- Max length to receive.
 * OUTPUT: iRecvBytes -- The length of received data in Bytes. -1 is Error. 0 is Client close.
 *================================================================================================*/
int TCPServer::Receive(char* cBuff, int iMaxRecv)
{
	if (m_client == INVALID_SOCKET) return -1;
	int iRecvBytes = SOCKET_ERROR;
	iRecvBytes = recv(m_client, cBuff, iMaxRecv, 0);
	return iRecvBytes;
}

/**************************************************************************************************
 * TCPServer::Close -- Close TCPServer.
 * 
 *     Close TCPServer and release socket resource.
 *================================================================================================*/
bool TCPServer::Close()
{
	if (m_client != INVALID_SOCKET)
	{
		closesocket(m_client);
		m_client = INVALID_SOCKET;
	}
	if (m_socket != INVALID_SOCKET)
	{
		closesocket(m_socket);
		m_socket = INVALID_SOCKET;
	}
	WSACleanup();
	return true;
}