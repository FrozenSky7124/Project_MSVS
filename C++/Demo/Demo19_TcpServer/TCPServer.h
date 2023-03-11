/**********************************************************************
 * Name:      FSNet_TCPServer                                         *
 * Function:  A simple TCP server class                               *
 * Author:    FrozenSky                                               *
 * Created:   2022-11-24                                              *
 * Updated:   2023-03-11                                              *
 * Copyright: FrozenSky@NTSS                                          *
 * License:   MIT                                                     *
 *====================================================================*/

#ifndef __TCP_SERVER_H__
#define __TCP_SERVER_H__

#include <WinSock2.h>

namespace FSNet
{


/*
 * Class: TCPServer
 */
class TCPServer
{
public:
	TCPServer();
	~TCPServer();
	bool StartUp();
	bool Open();
	bool Bind(const char* ip, int port);
	bool Listen(int backlog);
	bool Accept();
	int  Receive(char* cBuff, int iMaxRecv);
	bool Close();
protected:
private:
	WSADATA m_wsaData;
	SOCKET m_socket;
	SOCKADDR_IN m_socketAddr;
	SOCKET m_client;
	SOCKADDR_IN m_clientAddr;
};


}

#endif //__TCP_SERVER_H__