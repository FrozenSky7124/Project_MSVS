#include<iostream>

using namespace std;

void main10_21()
{
	int a = 10;
	int &b = a;
	b = 11;
	printf("\na:%d", a);
	printf("\n&a:%x", &a);
	printf("\n&b:%x", &b);
	system("pause");
}