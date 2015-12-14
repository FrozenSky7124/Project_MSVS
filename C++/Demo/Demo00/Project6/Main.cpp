#include <iostream>
#include "Point.h"
#include "Rectangle.h"

using namespace std;

int main()
{
	Point p1(0, 0);
	Point p2(10, 11);
	Rectangle r1(p1, p2);
	cout << "Distence: " << p1.getDist(p2) << endl;
	cout << "Rectangle Area: " << r1.getArea() << endl;
	system("pause");
	return 0;
}