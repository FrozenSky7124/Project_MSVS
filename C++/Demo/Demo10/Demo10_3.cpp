#include<iostream>

using namespace std;

int test1()
{
	int a = 123;
	return a;
}

int& test2() //����ֵ���������ͣ�Ҳ���ǳ���ָ�롣
{
	int a = 123;
	return a;
}
void main()
{
	int b1 = 0, b2 = 0;
	b1 = test1();
	b2 = test2(); //C++���������Զ�ִ��*������
	int& b3 = test2();
	printf("\nb1=%d", b1);
	printf("\nb2=%d", b2);
	printf("\nb3=%d", b3);
	system("pause");
}