#pragma once
#include "MyPoint.h"
class MyCircle
{
private:
	double m_x, m_y;
	double m_r;
public:
	void setCircle(double x, double y, double r);
	char* judge(double x, double y);
	char* judge(MyPoint &p);
};

