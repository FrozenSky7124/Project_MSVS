/*
数组类 [] == != 操作重载
*/
#include "iostream"
#include "Array.h"
using namespace std;

int main()
{
	Array a1(10);

	for (int i = 0; i < a1.length(); i++)
	{
		//a1.setData(i, i);  //a1[i] = i; int& operator[](int i);
		a1[i] = i;
	}

	for (int i = 0; i < a1.length(); i++)
	{
		// printf("array %d: %d\n", i, a1.getData(i));
		// printf("array %d: %d\n", i, a1[i]));
		cout << a1[i] << endl;
	}

	Array a2 = a1;

	for (int i = 0; i < a2.length(); i++)
	{
		// printf("array %d: %d\n", i, a2.getData(i));
		cout << a2[i] << endl;
	}

	// 	if (a2==a1)
	// 	{
	// 		printf("相等");
	// 	}
	// 	else
	// 	{
	// 		printf("不相等");
	// 	}

	// 
	Array a3(100);
	a1 = a3 = a2; //执行=操作 void operator=(Array& a)
	for (int i = 0; i < a3.length(); i++)
	{
		printf("array %d: %d\n", i, a3[i]);
	}

	system("pause");
	return 0;
}
