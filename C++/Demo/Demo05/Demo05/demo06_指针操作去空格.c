#include<stdio.h>
#include<stdlib.h>
#include<string.h>

int trimSpace(char *str_ori, char *str_final)
{
	char *p = str_ori;
	if (str_ori == NULL || str_final == NULL)
	{
		printf("Error: str_ori == NULL or str_final == NULL\n");
		return -1;
	}
	else
	{
		//ȥ���ַ���β�ո�
		p = p + strlen(str_ori);
		while ((*p == ' ') || (*p == '\0'))
		{
			p = p--;
		}
		*(p + 1) = '\0';
		//ȥ���ַ���ͷ�ո�
		p = str_ori;
		while (*p == ' ')
		{
			p = p++;
		}
		strcpy(str_final, p);
		return 0;
	}
}

void main()
{
	char str[50] = "     Fate/Zero StayNight          ";
	char result[50] = "";
	trimSpace(str, result);
	printf("%s", result);
	system("pause");
}