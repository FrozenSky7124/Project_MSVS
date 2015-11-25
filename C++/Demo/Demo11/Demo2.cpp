/*
操作符重载技术
*/
#include <iostream>
using namespace std;

class Complex
{
public:
	Complex();
	Complex(int a, int b);
	~Complex();

	void printComplex()
	{
		cout << a << " " << b << endl;
	}
	Complex operator - (Complex &temp);
	
private:
	int a;
	int b;
	friend Complex operator + (Complex &c1, Complex &c2);
};

Complex::Complex()
{
}
Complex::Complex(int a, int b)
{
	this->a = a;
	this->b = b;
}
Complex::~Complex()
{
}

Complex operator + (Complex &c1, Complex &c2)
{
	Complex tmp;
	tmp.a = c1.a + c2.a;
	tmp.b = c1.b + c2.b;
	return tmp;
}
Complex Complex::operator - (Complex &temp)
{
	Complex tmp;
	tmp.a = this->a - temp.a;
	tmp.b = this->b - temp.b;
	return tmp;
}
void main2()
{
	Complex c1(1, 2), c2(3, 4);
	Complex c3;
	c3 = c1 + c2;
	c3.printComplex();
	Complex c4;
	c4 = c1 - c2;
	c4.printComplex();
	system("pause");
}