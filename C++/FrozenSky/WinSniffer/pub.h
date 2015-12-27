#pragma once

#ifndef AFX_PUB_H__4DA3ED97_E13E_4CD5_A1F1_B588852E7223__INCLUDED_
#define AFX_PUB_H__4DA3ED97_E13E_4CD5_A1F1_B588852E7223__INCLUDED_


#include <WinSock2.h>
#include <mstcpip.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#pragma comment(lib,"Ws2_32.lib")

//定义协议名称结构
typedef struct _PROTN2T
{
	int proto;
	char *pprototext;
} PROTN2T;

//协议数
#define PROTO_NUM 11

//IP包头结构
typedef struct _IPHEADER
{
	unsigned char header_len : 4;//头长度，4个位长
	unsigned char version : 4;//版本号，4个位长
	unsigned char tos;//服务类型（定义包的优先级）
	unsigned short total_len;//整个包的长度字节数
	unsigned short ident;//标识（用于分割IP包）
	unsigned short flags;//综合标志位（用于重组IP包 前3位标志，后13位分块偏移）
	unsigned char ttl;//生存周期
	unsigned char proto;//协议
	unsigned short checksum;//校验和
	unsigned int sourceIP;//源IP
	unsigned int destIP;//目标IP
} IPHEADER;

//TCP包头结构
struct TCPPacketHead
{
	WORD SourPort;//16位源端口
	WORD DestPort;//16位目的端口
	DWORD SeqNo;//32位序列号
	DWORD AckNo;//32位确认号
	BYTE HLen;//头长度
	BYTE Flag;//标识位 紧急（URG） 确认（ACK） 推送（PSH） 重置（RST） 同步（SYN） 完成（FIN）
	WORD WndSize;//16位窗口大小
	WORD ChkSum;//16位TCP校验和
	WORD UrgPtr;//16位紧急指针
};

//ICMP包头结构
struct ICMPPacketHead
{
	BYTE Type;//类型
	BYTE Code;//编码
	WORD ChkSum;//16位TCP校验和
};

//UDP包头结构
struct UDPPacketHead
{
	WORD SourcePort;//源端口
	WORD DestPort;//目的端口
	WORD Len;//长度
	WORD ChkSum;//16位TCP校验和
};

//UDP头长度
#define UDP_HEAD_LEN 8
#define PSEUDO_HEAD_LEN 12

//ICMP头长度
#define ICMP_HEAD_LEN 4

char *Get_proto_name(unsigned char proto);//获得协议名称

void PrintByte(const char *Buf, size_t BufSize);//将二进制数转换为十六进制字符串

int SocketCreate(SOCKET &sock, const char *IPAddr, unsigned short Port);

int SinfferReceive(SOCKET &sock, bool ShowByte = false);

#endif
