#include "TCPServer.h"
#include <iostream>
#include <ws2tcpip.h>

#pragma comment (lib, "Ws2_32.lib")

using namespace FSNet;

TCPServer::TCPServer()
{
	m_socket = INVALID_SOCKET;
	memset(&m_socketAddr, 0, sizeof(SOCKADDR_IN));
	m_client = INVALID_SOCKET;
	memset(&m_clientAddr, 0, sizeof(SOCKADDR_IN));
}

TCPServer::~TCPServer()
{
	if (m_socket != INVALID_SOCKET)
	{
		closesocket(m_socket);
		m_socket = INVALID_SOCKET;
	}
	WSACleanup();
}

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