#include "iostream"
using namespace std;

void  swap(int &a, int &b)
{
	int c;
	c = a;
	a = b;
	b = c;
}

void  swap(float &a, float &b)
{
	float c;
	c = a;
	a = b;
	b = c;
}
void main11()
{
	int a = 1, b = 2;
	swap(a, b);
	float a1 = 1, b1 = 2;
	swap(a1, b1);
	system("pause");
}

template<typename T>
void swap2(T &a, T &b)
{
	T c;
	c = a;
	a = b;
	b = c;
}

void  main111()
{

	int x = 1, y = 2;
	swap2(x, y);
	printf("x:%d y:%d \n", x, y);

	float x1 = 1.0, y1 = 2.0;

	swap2<float>(x1, y1);
	printf("x1:%f y1:%f \n", x1, y1);
	system("pause");
}

