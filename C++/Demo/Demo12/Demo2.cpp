/*
1 ����ģ���������ͨ����һ��������
2 C++���������ȿ�����ͨ����
3 �������ģ����Բ���һ�����õ�ƥ�䣬��ôѡ��ģ��
4 ����ͨ����ģ��ʵ���б���﷨�޶�������ֻͨ��ģ��ƥ��
*/

/*
����ģ�岻�����Զ�����ת��
��ͨ�����ܹ������Զ�����ת��
*/

#include <iostream>
using namespace std;


int Max(int a, int b)
{
	cout << "int Max(int a, int b)" << endl;
	return a > b ? a : b;
}

template<typename T>
T Max(T a, T b)
{
	cout << "T Max(T a, T b)" << endl;
	return a > b ? a : b;
}

template<typename T>
T Max(T a, T b, T c)
{
	cout << "T Max(T a, T b, T c)" << endl;
	return Max(Max(a, b), c);
}


void main333()
{
	int a = 1;
	int b = 2;

	cout << Max(a, b) << endl;
	cout << Max<>(a, b) << endl;

	cout << Max(3.0, 4.0) << endl;

	cout << Max(5.0, 6.0, 7.0) << endl;

	cout << Max('a', 100) << endl;
	system("pause");
	return;
}
