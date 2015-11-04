#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

typedef struct _SCK_HANDLE {
	char	version[16];
	char	serverip[16];
	int		serverport;
	char*	pBuf;
	int		buflen;
}SCK_HANDLE;

//------------------第一套api接口---Begin--------------------------------//
//客户端初始化 获取handle上下
__declspec(dllexport)
int cltSocketInit(void **handle /*out*/)
{
	int ret = 0;
	SCK_HANDLE *sh = NULL;
	sh = (SCK_HANDLE *)malloc(sizeof(SCK_HANDLE));
	if (sh == NULL)
	{
		printf("Error : sh = (SCK_HANDLE *)malloc(sizeof(SCK_HANDLE));");
		ret = -1;
		return ret;
	}
	strcpy(sh->version, "1.0.0");
	strcpy(sh->serverip, "192.168.1.111");
	sh->serverport = 7124;
	*handle = sh;
	return ret;
}

//客户端发报文
__declspec(dllexport)
int cltSocketSend(void *handle /*in*/, unsigned char *buf /*in*/, int buflen /*in*/)
{
	int ret = 0;
	SCK_HANDLE *sh = NULL;
	if (handle == NULL | buf == NULL | buflen > 1024 * 1024)
	{
		printf("Error : if (handle == NULL | buf == NULL | buflen > 1024 * 1024)");
		ret = -1;
		return ret;
	}
	sh = (SCK_HANDLE*)handle;
	sh->pBuf = (char*)malloc(buflen*sizeof(char));
	memcpy(sh->pBuf, buf, buflen);
	sh->buflen = buflen;
	return ret;
}

//客户端收报文
__declspec(dllexport)
int cltSocketRev(void *handle /*in*/, unsigned char *buf /*in*/, int *buflen /*in out*/)
{
	int ret = 0;
	SCK_HANDLE *sh = NULL;
	if (handle == NULL | buf == NULL | buflen == NULL)
	{
		printf("Error : if (handle == NULL | buf == NULL | buflen == NULL)");
		ret = -1;
		return ret;
	}
	sh = (SCK_HANDLE*)handle;
	memcpy(buf, sh->pBuf, sh->buflen);
	*buflen = sh->buflen;
	return ret;
}

//客户端释放资源
__declspec(dllexport)
int cltSocketDestory(void *handle/*in*/)
{
	int ret = 0;
	SCK_HANDLE *sh = NULL;
	if (handle == NULL)
	{
		ret = -1;
		printf("Error : if (handle == NULL)");
		return ret;
	}
	sh = (SCK_HANDLE*)handle;
	if (sh->pBuf != NULL)
	{
		free(sh->pBuf);
		sh->pBuf = NULL;
	}
	free(sh);
	return ret;
}
//------------------第一套api接口---End-----------------------------------//