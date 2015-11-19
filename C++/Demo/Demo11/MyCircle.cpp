#include "MyCircle.h"

void MyCircle::setCircle(double x, double y, double r)
{
	m_x = x;
	m_y = y;
	m_r = r;
}

char * MyCircle::judge(double x, double y)
{
	double s = (m_x - x)*(m_x - x) + (m_y - y)*(m_y - y) - m_r*m_r;
	if (s > 0)
	{
		return "Point out of Circle.";
	}
	else
	{
		return "Point in the Circle.";
	}
}

char * MyCircle::judge(MyPoint & p)
{
	double s = (m_x - p.getX())*(m_x - p.getX()) + (m_y - p.getY())*(m_y - p.getY()) - m_r*m_r;
	if (s > 0)
	{
		return "Point out of Circle.";
	}
	else
	{
		return "Point in the Circle.";
	}
}
