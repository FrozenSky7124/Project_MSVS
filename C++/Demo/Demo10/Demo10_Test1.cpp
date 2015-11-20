/*
1 编写一个业务函数，实现字符串（前后各有三个空格，单词前后也均有空格） "   i am student, you are teacher.   " ，
各个单词首字符大写，结果如下 "   I Am Student, You Are Teacher.   " ，
*/
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <string.h>

using namespace std;

int GetString(const char *str1 /*in*/, char *str2 /*in*/);
int GetString_Adv(const char *str1/*in*/, char **str2 /*out*/);
int GetString_Adv_Free1(char *str2);
int GetString_Adv_Free2(char **str2);

int GetString(const char *str1 /*in*/, char *str2 /*in*/)
{
	int ret = 0;
	char* pPre = NULL;
	char* pCur = NULL;
	if (str1 == NULL || str2 == NULL)
	{
		ret = -1;
		return ret;
	}
	strcpy(str2, str1);
	pPre = str2;
	pCur = str2 + 1;
	while (*pCur != '\0')
	{
		if (isspace(*pPre) && isalpha(*pCur))
		{
			*pCur = *pCur - 32;
		}
		pCur++;
		pPre++;
	}
	return ret;
}

int GetString_Adv(const char *str1/*in*/, char **str2 /*out*/)
{
	int ret = 0;
	char *tmp = NULL;
	char* pPre = NULL;
	char* pCur = NULL;
	if (str1 == NULL || str2 == NULL)
	{
		ret = -1;
		return ret;
	}
	tmp = (char*)malloc(sizeof(char)*(strlen(str1) + 1));
	strcpy(tmp, str1);
	pPre = tmp;
	pCur = tmp + 1;
	while (*pCur != '\0')
	{
		if (isspace(*pPre) && isalpha(*pCur))
		{
			*pCur = *pCur - 32;
		}
		pCur++;
		pPre++;
	}
	*str2 = tmp;
	return ret;
}

int GetString_Adv_Free1(char *str2)
{
	int ret = 0;
	if (str2 != NULL)
	{
		free(str2);
	}
	return ret;
}

int GetString_Adv_Free2(char **str3)
{
	int ret = 0;
	if (str3 == NULL)
	{
		return -1;
	}
	if (*str3 != NULL)
	{
		free(*str3);
		*str3 = NULL;
	}
	return ret;
}

int main_Test1()
{
	int ret = 0;
	char* str1 = "   i am student, you are teacher.   ";
	char str2[1024] = { 0 };
	char *str3 = NULL;
	ret = GetString(str1 /*in*/, str2 /*in*/);
	cout << "1:" << str2 << endl;
	ret = GetString_Adv(str1/*in*/, &str3 /*out*/);
	cout << "2:" << str3 << endl;
	//ret = GetString_Adv_Free1(str2);
	ret = GetString_Adv_Free2(&str3);
	cout << "Free..." << endl;
	system("pause");
	return ret;
}