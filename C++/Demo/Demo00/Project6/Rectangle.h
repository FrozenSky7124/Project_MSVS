#pragma once
#include "Point.h"

class Rectangle
{
private:
	Point pl;
	Point pr;
public:
	Rectangle(Point pl, Point pr);
	~Rectangle();
	int getArea();
};

