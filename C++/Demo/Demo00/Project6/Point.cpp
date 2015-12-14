#include <iostream>
#include <math.h>
#include "Point.h"

using namespace std;

Point::Point(int x, int y)
{
	this->x = x;
	this->y = y;
}

Point::~Point()
{
	cout << "~Point" << endl;
}

int Point::getX()
{
	return this->x;
}

int Point::getY()
{
	return this->y;
}

float Point::getDist(Point &p)
{
	return sqrt(float((p.getX() - this->x)*(p.getX() - this->x) + (p.getY() - this->y)*(p.getY() - this->y)));
}
