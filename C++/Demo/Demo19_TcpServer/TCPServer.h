#ifndef __TCP_SERVER_H__
#define __TCP_SERVER_H__

#include <WinSock2.h>

namespace FSNet
{

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