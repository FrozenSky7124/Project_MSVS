#include<iostream>

using namespace std;

void main_10_1()
{
	double r;
	double s;
	cout << "Microsoft Visual Studio Community 2015 \n";
	cout << "Enter R:";
	cin >> r;
	s = 3.1415926*r*r;
	cout << "S:" << s;
	system("pause");
}
class circle
{
private:
	double r;
public:
	void setR(double tmp)
	{
		r = tmp;
	}
	double getR()
	{
		return r;
	}
	double getS()
	{
		return 3.14*r*r;
	}
};

void main()
{
	circle c;
	double r;
	cout << "Microsoft Visual Studio Community 2015 \n";
	cout << "Enter R:";
	cin >> r;
	c.setR(r);
	cout << "S:" << c.getS();
	system("pause");
}