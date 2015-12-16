#include <iostream>

#define PI 3.1415

using namespace std;

class Sharp
{
private:
public:
	Sharp()
	{
		cout << "Sharp" << endl;
	}
	~Sharp()
	{
		cout << "~Sharp" << endl;
	}
	virtual float getArea() = 0;
};
class Rectangle :public Sharp
{
private:
	int a;
	int b;
public:
	Rectangle(int a = 0, int b = 0)
	{
		cout << "Rectangle" << endl;
		this->a = a;
		this->b = b;
	}
	~Rectangle()
	{
		cout << "~Rectangle" << endl;
	}
	float getArea()
	{
		return a*b;
	}
};
class Circle :public Sharp
{
private:
	int x;
	int y;
	int r;
public:
	Circle(int a = 0, int b = 0, int r = 0)
	{
		cout << "Circle" << endl;
		this->x = x;
		this->y = y;
		this->r = r;
	}
	~Circle()
	{
		cout << "~Circle" << endl;
	}
	float getArea()
	{
		return PI*r*r;
	}
};
class Square :public Rectangle
{
private:
	int w;
public:
	Square(int w = 0)
	{
		cout << "Square" << endl;
		this->w = w;
	}
	~Square()
	{
		cout << "~Square" << endl;
	}
	float getArea()
	{
		return w*w;
	}
};

void printArea(Sharp &sharp)
{
	cout << sharp.getArea() << endl;
	return;
}

int main()
{
	//Rectangle r(5, 5);
	//Square s(5);
	Circle c(5, 5, 10);
	printArea(c);
	system("pause");
	return 0;
}