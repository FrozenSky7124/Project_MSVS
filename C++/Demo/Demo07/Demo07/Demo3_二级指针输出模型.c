#include<stdio.h>
#include<stdlib.h>
#include<string.h>

void main1()
{
	int a = 0;
	int *p = NULL;
	p = &a;
	*p = 10;

	printf("a:%d \n", a);
	
	system("pause");
}
void changeP(char **p)
{
	*p = 0x22222;
}
void main_demo3()
{
	char *p = NULL;
	p = 0x10010;
	changeP(&p);
	printf("p:%x \n", p);

	system("pause");
}