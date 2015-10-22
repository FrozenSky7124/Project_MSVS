#include<stdio.h>
#include<stdlib.h>
#include<string.h>

int createBuf(char **p)
{
	char *tmp = (char *)malloc(5);
	memset(tmp, 0, 5);
	memset(tmp, 97, 4);
	*p = tmp;
	return;
}
void main_demo4()
{
	char *p = NULL;
	createBuf(&p);
	printf("%s", p);

	if (p != NULL) free(p);
	system("pause");
}