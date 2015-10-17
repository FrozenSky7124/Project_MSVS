#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char * getstring1()
{
	char *p1 = "abcde1";
	return p1;
}

char * getstring2()
{
	char *p2 = "abcde1";
	return p2;
}
/*
void main()
{
	char *p1 = getstring1();
	char *p2 = getstring2();
	printf("p1:%s\n", p1);
	printf("p2:%s\n", p2);
	printf("p1:%d\n", p1);
	printf("p2:%d\n", p2);
	printf("p1.size = %d\n", sizeof(p1));
	printf("p2.size = %d\n", sizeof(p2));
	system("pause");
}
*/