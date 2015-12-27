#pragma once

#ifndef AFX_PUB_H__4DA3ED97_E13E_4CD5_A1F1_B588852E7223__INCLUDED_
#define AFX_PUB_H__4DA3ED97_E13E_4CD5_A1F1_B588852E7223__INCLUDED_


#include <WinSock2.h>
#include <mstcpip.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#pragma comment(lib,"Ws2_32.lib")

//����Э�����ƽṹ
typedef struct _PROTN2T
{
	int proto;
	char *pprototext;
} PROTN2T;

//Э����
#define PROTO_NUM 11

//IP��ͷ�ṹ
typedef struct _IPHEADER
{
	unsigned char header_len : 4;//ͷ���ȣ�4��λ��
	unsigned char version : 4;//�汾�ţ�4��λ��
	unsigned char tos;//�������ͣ�����������ȼ���
	unsigned short total_len;//�������ĳ����ֽ���
	unsigned short ident;//��ʶ�����ڷָ�IP����
	unsigned short flags;//�ۺϱ�־λ����������IP�� ǰ3λ��־����13λ�ֿ�ƫ�ƣ�
	unsigned char ttl;//��������
	unsigned char proto;//Э��
	unsigned short checksum;//У���
	unsigned int sourceIP;//ԴIP
	unsigned int destIP;//Ŀ��IP
} IPHEADER;

//TCP��ͷ�ṹ
struct TCPPacketHead
{
	WORD SourPort;//16λԴ�˿�
	WORD DestPort;//16λĿ�Ķ˿�
	DWORD SeqNo;//32λ���к�
	DWORD AckNo;//32λȷ�Ϻ�
	BYTE HLen;//ͷ����
	BYTE Flag;//��ʶλ ������URG�� ȷ�ϣ�ACK�� ���ͣ�PSH�� ���ã�RST�� ͬ����SYN�� ��ɣ�FIN��
	WORD WndSize;//16λ���ڴ�С
	WORD ChkSum;//16λTCPУ���
	WORD UrgPtr;//16λ����ָ��
};

//ICMP��ͷ�ṹ
struct ICMPPacketHead
{
	BYTE Type;//����
	BYTE Code;//����
	WORD ChkSum;//16λTCPУ���
};

//UDP��ͷ�ṹ
struct UDPPacketHead
{
	WORD SourcePort;//Դ�˿�
	WORD DestPort;//Ŀ�Ķ˿�
	WORD Len;//����
	WORD ChkSum;//16λTCPУ���
};

//UDPͷ����
#define UDP_HEAD_LEN 8
#define PSEUDO_HEAD_LEN 12

//ICMPͷ����
#define ICMP_HEAD_LEN 4

char *Get_proto_name(unsigned char proto);//���Э������

void PrintByte(const char *Buf, size_t BufSize);//����������ת��Ϊʮ�������ַ���

int SocketCreate(SOCKET &sock, const char *IPAddr, unsigned short Port);

int SinfferReceive(SOCKET &sock, bool ShowByte = false);

#endif
