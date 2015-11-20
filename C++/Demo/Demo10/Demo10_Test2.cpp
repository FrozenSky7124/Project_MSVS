/*
��дһ��ҵ������ʵ�ְ��ж�ȡ�ļ��������ݰ��յ������ڴ�ģ�ʹ�����ݴ�����������ͨ����������������
��д�ڴ��ͷź�����
*/
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>

using namespace std;

int readFile(char *pFilePath /*in*/, char ***pOut /*in out*/, int *lineNum /*in out*/)
{
	int ret = 0;
	int tmplineNum = 0;
	FILE *fp = NULL;
	char *pTmp = NULL;
	char **tmpFile = NULL;
	char lineBuf[1024] = { 0 };
	if (pFilePath == NULL || lineNum == NULL)
	{
		ret = -1;
		return ret;
	}
	fp = fopen(pFilePath, "r");
	if (fp == NULL)
	{
		ret = -1;
		return ret;
	}
	while (!feof(fp))
	{
		memset(lineBuf, 0, sizeof(lineBuf));
		pTmp = fgets(lineBuf, 1024, fp);
		if (pTmp != NULL)
		{
			tmplineNum++;
		}
	}
	tmpFile = (char**)malloc((tmplineNum)*sizeof(char*));
	fseek(fp, 0L, SEEK_SET); //�����ļ�ָ��
	tmplineNum = 0;
	while (!feof(fp))
	{
		memset(lineBuf, 0, sizeof(lineBuf));
		pTmp = fgets(lineBuf, 1024, fp);
		if (pTmp != NULL)
		{
			int tmpLen = strlen(lineBuf);
			tmpFile[tmplineNum] = (char*)malloc((tmpLen + 1)*sizeof(char));
			strcpy(tmpFile[tmplineNum], lineBuf);
			cout << tmpFile[tmplineNum];
			tmplineNum++;
		}
	}
	*pOut = tmpFile;
	*lineNum = tmplineNum;
	if (fp != NULL) {
		fclose(fp);
	}
	return ret;
}

int readFile_Free(char **pIn, int lineNum)
{
	int i = 0;
	if (pIn == NULL)
	{
		return 0;
	}
	for (i = 0; i < lineNum; i++)
	{
		free(pIn[i]);
	}
	free(pIn);
	return 0;
}

int main()
{
	int result = 0;
	int i = 0;
	char *pFilepath = "D:/Project_MSVS/C++/Demo/Demo07/Debug/config.ini";
	char **pFile = NULL;
	int lineNum = 0;
	result = readFile(pFilepath /*in*/, &pFile /*in out*/, &lineNum /*in out*/);
	for (i = 0;i < lineNum;i++)
	{
		printf("%s", pFile[i]);
	}
	result = readFile_Free(pFile, lineNum);
	system("pause");
	return result;
}