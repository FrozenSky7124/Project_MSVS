#include <iostream>
#include "Rectangle.h"

using namespace std;

Rectangle::Rectangle(Point pl, Point pr)
{
	this->pl = pl;
	this->pr = pr;
}
Rectangle::~Rectangle()
{
	cout << "~Rectangle" << endl;
}

int Rectangle::getArea()
{
	return (pl.getX() - pr.getX())*(pl.getY() - pr.getY());
}
