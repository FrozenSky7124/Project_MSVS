#include <stdio.h>
#include <winsock.h>

#define LISTENPORT 17241 //定义接收端口
#define UDPPKGSIZE 32    //定义UDP数据包大小

#pragma comment(lib,"ws2_32.lib")

// UDP Socket
WSADATA m_wsaData;
SOCKET m_socket;
SOCKADDR_IN m_addrRecvin;

BOOL InitSocket()
{
	UINT iRst = 0;

	// 初始化SOCKET
	iRst = WSAStartup(MAKEWORD(2, 2), &m_wsaData);
	if (iRst != 0)
	{
		printf("WSAStartup failed: %d\n", iRst);
		return FALSE;
	}
	// 创建UDP_Socket
	m_socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (m_socket == INVALID_SOCKET)
	{
		printf("socket function failed with error = %d\n", WSAGetLastError());
		WSACleanup();
		return FALSE;
	}
	else
	{
		m_addrRecvin.sin_family = AF_INET;
		m_addrRecvin.sin_port = htons(LISTENPORT);
		m_addrRecvin.sin_addr.s_addr = htonl(INADDR_ANY);
		iRst = bind(m_socket, (SOCKADDR *)& m_addrRecvin, sizeof(m_addrRecvin));
		if (iRst != 0)
		{
			printf("bind failed with error %d\n", WSAGetLastError());
			return FALSE;
		}
		return TRUE;
	}
}

void MyPrint(char * pRecvBuf)
{
	if (strcmp("CLOSE_01", pRecvBuf) == 0)
	{
		printf("                           !         !                     \n");
		printf("                          ! !       ! !                    \n");
		printf("                         ! . !     ! . !                   \n");
		printf("                            ^^^^^^^^^ ^                    \n");
		printf("                          ^             ^                  \n");
		printf("                        ^  (0)       (0)  ^                \n");
		printf("                       ^        ""         ^               \n");
		printf("                      ^   ***************    ^             \n");
		printf("                    ^   *                 *   ^            \n");
		printf("                   ^   *   /\   /\   /\    *    ^          \n");
		printf("                  ^   *                     *    ^         \n");
		printf("                 ^   *   /\   /\   /\   /\   *    ^        \n");
		printf("                ^   *                         *    ^       \n");
		printf("                ^  *                           *   ^       \n");
		printf("                ^  *             1             *   ^       \n");
		printf("                 ^ *                           *  ^        \n");
		printf("                  ^*                           * ^         \n");
		printf("                   ^ *                        * ^          \n");
		printf("                   ^  *                      *  ^          \n");
		printf("                     ^  *       ) (         * ^            \n");
		printf("                         ^^^^^^^^ ^^^^^^^^^                \n");
	}
	if (strcmp("CLOSE_02", pRecvBuf) == 0)
	{
		printf("                           !         !                     \n");
		printf("                          ! !       ! !                    \n");
		printf("                         ! . !     ! . !                   \n");
		printf("                            ^^^^^^^^^ ^                    \n");
		printf("                          ^             ^                  \n");
		printf("                        ^  (0)       (0)  ^                \n");
		printf("                       ^        ""         ^               \n");
		printf("                      ^   ***************    ^             \n");
		printf("                    ^   *                 *   ^            \n");
		printf("                   ^   *   /\   /\   /\    *    ^          \n");
		printf("                  ^   *                     *    ^         \n");
		printf("                 ^   *   /\   /\   /\   /\   *    ^        \n");
		printf("                ^   *                         *    ^       \n");
		printf("                ^  *                           *   ^       \n");
		printf("                ^  *             2             *   ^       \n");
		printf("                 ^ *                           *  ^        \n");
		printf("                  ^*                           * ^         \n");
		printf("                   ^ *                        * ^          \n");
		printf("                   ^  *                      *  ^          \n");
		printf("                     ^  *       ) (         * ^            \n");
		printf("                         ^^^^^^^^ ^^^^^^^^^                \n");
	}
	if (strcmp("CLOSE_03", pRecvBuf) == 0)
	{
		printf("                           !         !                     \n");
		printf("                          ! !       ! !                    \n");
		printf("                         ! . !     ! . !                   \n");
		printf("                            ^^^^^^^^^ ^                    \n");
		printf("                          ^             ^                  \n");
		printf("                        ^  (0)       (0)  ^                \n");
		printf("                       ^        ""         ^               \n");
		printf("                      ^   ***************    ^             \n");
		printf("                    ^   *                 *   ^            \n");
		printf("                   ^   *   /\   /\   /\    *    ^          \n");
		printf("                  ^   *                     *    ^         \n");
		printf("                 ^   *   /\   /\   /\   /\   *    ^        \n");
		printf("                ^   *                         *    ^       \n");
		printf("                ^  *                           *   ^       \n");
		printf("                ^  *             3             *   ^       \n");
		printf("                 ^ *                           *  ^        \n");
		printf("                  ^*                           * ^         \n");
		printf("                   ^ *                        * ^          \n");
		printf("                   ^  *                      *  ^          \n");
		printf("                     ^  *       ) (         * ^            \n");
		printf("                         ^^^^^^^^ ^^^^^^^^^                \n");
	}
	if (strcmp("CLOSE_04", pRecvBuf) == 0)
	{
		printf("                           !         !                     \n");
		printf("                          ! !       ! !                    \n");
		printf("                         ! . !     ! . !                   \n");
		printf("                            ^^^^^^^^^ ^                    \n");
		printf("                          ^             ^                  \n");
		printf("                        ^  (0)       (0)  ^                \n");
		printf("                       ^        ""         ^               \n");
		printf("                      ^   ***************    ^             \n");
		printf("                    ^   *                 *   ^            \n");
		printf("                   ^   *   /\   /\   /\    *    ^          \n");
		printf("                  ^   *                     *    ^         \n");
		printf("                 ^   *   /\   /\   /\   /\   *    ^        \n");
		printf("                ^   *                         *    ^       \n");
		printf("                ^  *                           *   ^       \n");
		printf("                ^  *             4             *   ^       \n");
		printf("                 ^ *                           *  ^        \n");
		printf("                  ^*                           * ^         \n");
		printf("                   ^ *                        * ^          \n");
		printf("                   ^  *                      *  ^          \n");
		printf("                     ^  *       ) (         * ^            \n");
		printf("                         ^^^^^^^^ ^^^^^^^^^                \n");
	}
}

int main()
{

	BOOL bUDPState = FALSE;
	char cRecvBuf[UDPPKGSIZE];
	sockaddr_in addr_sender;
	int iSenderAddrSize = sizeof(addr_sender);
	int iRst = 0;

	// 初始化 UDP Socket
	bUDPState = InitSocket();
	if (FALSE == bUDPState)
	{
		printf("[UDP Socket] init faild.\n");
		Sleep(3000);
		return -1;
	}
	
	printf("[UDP Socket] init success.\n");
	
	while (1)
	{
		if ((iRst = recvfrom(m_socket, cRecvBuf, UDPPKGSIZE, 0, (SOCKADDR *)& addr_sender, &iSenderAddrSize)) != SOCKET_ERROR)
		{
			cRecvBuf[iRst] = '\0';
			printf("MESSAGE: %s\n", cRecvBuf);
			MyPrint(cRecvBuf);
		}
		else
		{
			printf("[UDP Socket] recvfrom function error!\n");
			Sleep(3000);
			break;
		}
	}

	// 关闭 UDP Socket
	iRst = closesocket(m_socket);
	if (iRst == SOCKET_ERROR)
		printf("[UDP Socket] closesocket failed with error = %d\n", WSAGetLastError());
	WSACleanup();
	return 0;
}