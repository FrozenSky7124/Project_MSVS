// WinSniffer.cpp : 定义控制台应用程序的入口点。
//
#include "stdafx.h"
#include "pub.h"

int main(int argc, char *argv[])
{
	if (argc < 3)
	{
		printf("Please enter IP Port [Byte].\n");
		return 0;
	}
	SOCKET sock;
	int iPort = atoi(argv[2]);
	int iRes = SocketCreate(sock, argv[1], (unsigned short)iPort);
	if (iRes != 0)
	{
		LPVOID lpMsgBuf;
		FormatMessage(
			FORMAT_MESSAGE_ALLOCATE_BUFFER |
			FORMAT_MESSAGE_FROM_SYSTEM |
			FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL,
			iRes,
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
			(LPTSTR)&lpMsgBuf,
			0,
			NULL
			);
		printf("Error:%d %s\n", iRes, (LPCTSTR)&lpMsgBuf);
		LocalFree(lpMsgBuf);
		return -1;
	}

	while (true)
	{
		if (argc == 4)
			iRes = SinfferReceive(sock, true);
		else
			iRes = SinfferReceive(sock);
		if (iRes != 0)
		{
			LPVOID lpMsgBuf;
			FormatMessage(
				FORMAT_MESSAGE_ALLOCATE_BUFFER |
				FORMAT_MESSAGE_FROM_SYSTEM |
				FORMAT_MESSAGE_IGNORE_INSERTS,
				NULL,
				iRes,
				MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
				(LPTSTR)&lpMsgBuf,
				0,
				NULL
				);
			printf("Error:%d %s\n", iRes, (LPCTSTR)lpMsgBuf);
			LocalFree(lpMsgBuf);
			return -1;
		}
	}

	return 0;
}

