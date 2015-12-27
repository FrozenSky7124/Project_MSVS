#include "stdafx.h"

char *Get_proto_name(unsigned char proto)//���Э������
{
	switch (proto)
	{
	case IPPROTO_IP:
		return "IP";
	case IPPROTO_ICMP:
		return "ICMP";
	case IPPROTO_IGMP:
		return "IGMP";
	case IPPROTO_GGP:
		return "GGP";
	case IPPROTO_TCP:
		return "TCP";
	case IPPROTO_PUP:
		return "PUP";
	case IPPROTO_UDP:
		return "UDP";
	case IPPROTO_IDP:
		return "IDP";
	case IPPROTO_ND:
		return "ND";
	case IPPROTO_RAW:
		return "RAW";
	case IPPROTO_MAX:
		return "MAX";
	default:
		return "UNKNOW";
	}
}

void PrintByte(const char *Buf, size_t BufSize)
{
	for (size_t i = 0;i < BufSize;i++)
	{
		printf("%.2x ", (unsigned char)Buf[i]);
	}
}

int SocketCreate(SOCKET &sock, const char *IPAddr, unsigned short Port)
{
	//��ʼ��WinSocket
	unsigned short wVersion;
	WSADATA wsaData;
	wVersion = MAKEWORD(1, 1);
	int iRes = WSAStartup(wVersion, &wsaData);
	if (iRes != 0)
		return iRes;

	//����ԭʼSocket
	sock = socket(AF_INET, SOCK_RAW, IPPROTO_IP);
	if (sock == INVALID_SOCKET)
		return WSAGetLastError();

	//���ó�ʱѡ��
	int iRecTime = 50000;
	if (setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, (const char *)&iRecTime, sizeof(iRecTime)) == -1)
		return WSAGetLastError();

	//����Socket�󶨵�ַ�Ͷ˿�
	sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(Port);
	addr.sin_addr.s_addr = inet_addr(IPAddr);
	if (bind(sock, (LPSOCKADDR)&addr, sizeof(addr)) == SOCKET_ERROR)
		return WSAGetLastError();

	DWORD dwBufferInLen = 1;
	DWORD dwBufferLen[10];
	DWORD dwBytesReturned = 0;
	//����WSAIoctl ����Socket���Խ�������IP��
	if (WSAIoctl(sock, SIO_RCVALL, &dwBufferInLen, sizeof(dwBufferInLen),
		&dwBufferLen, sizeof(dwBufferLen), &dwBytesReturned, NULL, NULL) == SOCKET_ERROR)
		return WSAGetLastError();
	return 0;
}

int SinfferReceive(SOCKET &sock, bool ShowByte)
{
	IPHEADER *ipHeader = NULL;
	TCPPacketHead *tcpHeader = NULL;
	ICMPPacketHead *icmpHeader = NULL;
	UDPPacketHead *udpHeader = NULL;
	BYTE *pData = NULL;
	char *pLastBuf = NULL;

	WORD wSrcPort, wDestPort;
	char sSrcIPAddr[32], sDestIPAddr[32], sProtoName[32];
	memset(sSrcIPAddr, 0, sizeof(sSrcIPAddr));
	memset(sDestIPAddr, 0, sizeof(sDestIPAddr));
	memset(sProtoName, 0, sizeof(sProtoName));
	in_addr inaddr;

	char sBuf[8192];//SocketĬ��BufferΪ8k
	char *pBuf = sBuf;
	memset(sBuf, 0, sizeof(sBuf));
	int iRes = recv(sock, sBuf, sizeof(sBuf), 0);//��������
	if (iRes == SOCKET_ERROR)
		return WSAGetLastError();

	ipHeader = (IPHEADER *)pBuf;//���IP��ͷָ��
	WORD iLen = ntohs(ipHeader->total_len);//���IP��ͷ�ܳ���

	while (true)
	{
		if (iLen <= iRes)
		{
			//���IP��Դ��ַ
			inaddr.S_un.S_addr = ipHeader->sourceIP;
			strcpy(sSrcIPAddr, inet_ntoa(inaddr));

			//���IP��Ŀ�ĵ�ַ
			inaddr.S_un.S_addr = ipHeader->destIP;
			strcpy(sDestIPAddr, inet_ntoa(inaddr));

			//���Э������
			strcpy(sProtoName, Get_proto_name(ipHeader->proto));

			//���IP��ͷ���ȣ�header_lenΪ4Bit���ݣ�������Ҫ���㣩
			int iHdrLen = ipHeader->header_len & 0xf;
			iHdrLen *= 4;
			//������ݵĳ���
			int iTotalLen = ntohs(ipHeader->total_len);
			iTotalLen -= iHdrLen;

			switch (ipHeader->proto)
			{
			case IPPROTO_ICMP:
			{
				icmpHeader = (ICMPPacketHead *)(sBuf + iHdrLen);
				pData = (BYTE *)icmpHeader + ICMP_HEAD_LEN;
				iTotalLen -= ICMP_HEAD_LEN;
				break;
			}
			case IPPROTO_TCP:
			{
				tcpHeader = (TCPPacketHead *)(sBuf + iHdrLen);
				//TCPԴ�˿�
				wSrcPort = ntohs(tcpHeader->SourPort);
				//TCPĿ��˿�
				wDestPort = ntohs(tcpHeader->DestPort);
				iHdrLen = tcpHeader->HLen >> 4;
				iHdrLen *= 4;
				pData = ((BYTE *)tcpHeader) + iHdrLen;
				iTotalLen -= iHdrLen;
				break;
			}
			case IPPROTO_UDP:
			{
				udpHeader = (UDPPacketHead *)(sBuf + iHdrLen);
				wSrcPort = ntohs(udpHeader->SourcePort);
				wDestPort = ntohs(udpHeader->DestPort);
				pData = ((BYTE *)udpHeader) + UDP_HEAD_LEN;
				iTotalLen -= UDP_HEAD_LEN;
				break;
			}
			}

			static unsigned int iSequence = 0;
			int ipLen = strlen(sDestIPAddr);
			if (strncmp(sDestIPAddr + ipLen - 3, "255",3) != 0)//���˵�*.*.*.255��UDP�㲥
			{
				iSequence++;
				printf("--------------- Begin %.10u ---------------\n", iSequence);
				printf("ProtoName:%s\nSrcAddr:%s\nDestAddr:%s\nSrcPort:%d\nDestPort:%d\n",
					sProtoName, sSrcIPAddr, sDestIPAddr, wSrcPort, wDestPort);

				//����Internet�����Э�飨IGMP��������
				if (strcmp(sProtoName, "IGMP") != 0)
				{
					if (ShowByte)
					{
						printf("Bytes:\n");
						PrintByte((char *)pData, iTotalLen);
					}
					printf("\nASCII:\n%s\n", (char *)pData);
				}
				printf("---------------  End  %.10u ---------------\n\n", iSequence);
			}

			if (iLen < iRes)
			{
				iRes -= iLen;
				pBuf += iLen;
				ipHeader = (IPHEADER *)(pBuf);
			}
			else
			{
				break;
			}
		}
		else
		{
			int iLast = iLen - iRes;
			if (pLastBuf)
				delete []pLastBuf;
			pLastBuf = new char[iLen];
			int iReaden = iRes;
			memcpy(pLastBuf, pBuf, iReaden);
			iRes = recv(sock, pLastBuf + iReaden, iLast, 0);
			if (iRes == SOCKET_ERROR)
				return WSAGetLastError();

			pBuf = pLastBuf;
			ipHeader = (IPHEADER *)pBuf;
			if (iRes == iLast)
				iRes = iLen;
			else
			{
				//��ʣ�������
				iReaden += iRes;
				iLast -= iRes;
				while (true)
				{
					iRes = recv(sock, pLastBuf + iReaden, iLast, 0);
					if (iRes == SOCKET_ERROR)
						return WSAGetLastError();
					iReaden += iRes;
					iLast -= iRes;
					if (iLast <= 0)
						break;
				}
			}
		}
	}
	return 0;
}