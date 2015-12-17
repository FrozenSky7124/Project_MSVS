#include <iostream>

using namespace std;

class Point
{
private:
	int x;
	int y;
public:
	Point(int x, int y)
	{
		this->x = x;
		this->y = y;
	}
	~Point()
	{
		;
	}
	void print();
	friend Point& operator++ (Point& a);
	friend Point& operator-- (Point& a);
	friend Point operator++ (Point& a, int);
	friend Point operator-- (Point& a, int);
};

Point& operator++ (Point& a)
{
	a.x++;
	a.y++;
	return a;
}
Point& operator-- (Point& a)
{
	a.x--;
	a.y--;
	return a;
}
Point operator++ (Point& a, int)
{
	Point tmp = a;
	a.x++;
	a.y++;
	return tmp;
}
Point operator-- (Point& a, int)
{
	Point tmp = a;
	a.x--;
	a.y--;
	return tmp;
}
void Point::print()
{
	cout << this->x << " " << this->y << " " << endl;
}

int main()
{
	Point p1(0, 0);
	Point p2(5, 5);
	p1.print();
	p2.print();
	(p1++).print();
	(p2--).print();
	p1.print();
	p2.print();
	system("pause");
	return 0;
}