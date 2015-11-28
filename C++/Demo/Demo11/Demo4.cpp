#define _CRT_SECURE_NO_WARNINGS
#include "iostream"
#include "SocketImp1.h"
#include "SocketImp2.h"
#include "SocketProtocol.h"
using namespace std;


//主框架
void mainOP01()
{
	SocketIF *sIf  = new SocketImp1();

	unsigned char buf[1024]; 
	strcpy((char *)buf, "ddddddddddddddsssssssssssssssssssss");
	 int buflen = 10;

	 unsigned char out[1024];
	 int outlen = 10;
	sIf->cltSocketInit();
	sIf->cltSocketSend(buf, buflen);
	sIf->cltSocketRev(out, &outlen);
	sIf->cltSocketDestory();

}

int mainOP02(SocketIF *sIf, unsigned char *in, int inlen, unsigned char *out, int *outlen)
{
	int ret = 0;
	ret = sIf->cltSocketInit();
	ret = sIf->cltSocketSend(in, inlen);
	ret = sIf->cltSocketRev(out, outlen);
	ret = sIf->cltSocketDestory();
	return ret;

}
void main03()
{
	SocketIF *sIf  = new SocketImp2();

	unsigned char buf[1024]; 
	strcpy((char *)buf, "ddddddddddddddsssssssssssssssssssss");
	int buflen = 10;

	unsigned char out[1024] = {0};
	int outlen = 10;

	mainOP02(sIf, buf, buflen, out, &outlen);
	printf("out:%s", out);
	delete sIf;

	system("pause");
}
class MainOp
{
public:
	MainOp(SocketIF *sIf)
	{
		this->sIf = sIf;
	}

	MainOp()
	{
		;
	}
	void setSIF(SocketIF *sIf)
	{
		this->sIf = sIf;
	}
	SocketIF *getSIF()
	{
		return this->sIf;
	}
public:
	int Op1(unsigned char *in, int inlen, unsigned char *out, int *outlen)
	{
		int ret = 0;
		ret = this->sIf->cltSocketInit();
		ret = this->sIf->cltSocketSend(in, inlen);
		ret = this->sIf->cltSocketRev(out, outlen);
		ret = this->sIf->cltSocketDestory();
		return ret;

	}
	
	int Op2(SocketIF *sIf, unsigned char *in, int inlen, unsigned char *out, int *outlen)
	{
		int ret = 0;
		ret = sIf->cltSocketInit();
		ret = sIf->cltSocketSend(in, inlen);
		ret = sIf->cltSocketRev(out, outlen);
		ret = sIf->cltSocketDestory();
		return ret;

	}
private:
	SocketIF *sIf;
};

//注入例子
void main()
{
	SocketIF *sIf  = new SocketImp2();
	unsigned char buf[1024]; 
	strcpy((char *)buf, "ddddddddddddddsssssssssssssssssssss");
	int buflen = 10;

	unsigned char out[1024] = {0};
	int outlen = 10;

	MainOp mop(sIf);
	mop.Op1(buf, buflen, out, &outlen);
	printf("out:%s", out);
	delete sIf;
	system("pause");
}

//弱关联
void main444()
{
	SocketIF *sIf  = new SocketImp2();
	unsigned char buf[1024]; 
	strcpy((char *)buf, "ddddddddddddddsssssssssssssssssssss");
	int buflen = 10;

	unsigned char out[1024] = {0};
	int outlen = 10;

	MainOp mop;
	mop.Op2(sIf, buf, buflen, out, &outlen);
	printf("out:%s", out);
	delete sIf;
	system("pause");
}

