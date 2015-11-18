/***************************
Describe:函数重载、函数指针
***************************/
#include <iostream>

using namespace std;

typedef int(Array)[10];	//定义数组类型
typedef int(*pArray)[10];	//定义指向数组类型的指针类型
typedef int(*pFunc)(int a, int b);	//定义指向函数类型的指针类型

int func(int x)
{
	return x;
}
int func(int a, int b)
{
	return a + b;
}
int func(const char* s)
{
	return strlen(s);
}

int main10_41()
{
	int c = 0;
	pFunc pfunc = NULL;
	pfunc = func;
	c = pfunc(1, 2);
	printf("c=%d\n", c);
	//Array myArray;	//int myArray[10];
	//pArray pmyArray = NULL;	//Array *pmyArray;
	system("pause");
	return 0;
}