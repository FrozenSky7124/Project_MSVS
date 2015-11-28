#include "SocketImp1.h"
#include "iostream"
using namespace std;


SocketImp1::SocketImp1(void)
{
	buf = NULL;
	 buflen = 0;;
}


SocketImp1::~SocketImp1(void)
{
	if (this->buf )
		free(this->buf);
	this->buflen = 0;
	printf("SocketImp1��������do\n");
}

int SocketImp1::cltSocketInit()
{
	buf = NULL;
	buflen = 0;
	return 0;
}

//�ͻ��˷�����
int SocketImp1::cltSocketSend( unsigned char *buf ,  int buflen )
{
	if (buf == NULL || buflen>323432432432)
	{
		return -1;
	}
	this->buf = (unsigned char *)malloc(buflen*sizeof(char));
	if (this->buf  == NULL)
	{
		return -2;
	}
	memcpy(this->buf, buf, buflen);
	this->buflen  = buflen;

	return 0;
}

//�ͻ����ձ���
int SocketImp1::cltSocketRev( unsigned char *buf , int *buflen )
{
	if (buf == NULL || buflen == NULL)
	{
		return -1;
	}
	memcpy(buf, this->buf, this->buflen);
	*buflen = this->buflen;
	return 0;
}

//�ͻ����ͷ���Դ
int SocketImp1::cltSocketDestory()
{
	buf = NULL;
	buflen = 0;
	return 0;
}
