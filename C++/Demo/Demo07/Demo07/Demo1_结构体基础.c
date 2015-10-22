#include<stdio.h>
#include<stdlib.h>
#include<string.h>

typedef struct _Teacher
{
	char name[64];
	int age;
} Teacher;

void main7_2()
{
	Teacher t1;
	strcpy(t1.name, "FrozenSky");
	t1.age = 21;
	Teacher *p = NULL;
	//printf("%d\n", sizeof(char));
	//printf("%d\n", sizeof(int));
	//printf("%d\n", sizeof(short));
	p = &t1;
	int *i = (int*)malloc(10 * sizeof(int));
	int j = 0;
	for (j = 0; j < 10; j++)
	{
		*(i + j) = j;
	}
	for (j = 0; j < 10; j++)
	{
		printf("%d ", *(i + j));
	}
	system("pause");
}