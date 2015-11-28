#include "SocketImp2.h"
#include "iostream"
using namespace std;


SocketImp2::SocketImp2(void)
{
	buf = NULL;
	buflen = 0;;
}


SocketImp2::~SocketImp2(void)
{
	if (this->buf )
		free(this->buf);
	this->buflen = 0;
	printf("SocketImp2��������do\n");
}

int SocketImp2::cltSocketInit()
{
	buf = NULL;
	buflen = 0;
	return 0;
}

//�ͻ��˷�����
int SocketImp2::cltSocketSend( unsigned char *buf ,  int buflen )
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
int SocketImp2::cltSocketRev( unsigned char *buf , int *buflen )
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
int SocketImp2::cltSocketDestory()
{
	buf = NULL;
	buflen = 0;
	return 0;
}
