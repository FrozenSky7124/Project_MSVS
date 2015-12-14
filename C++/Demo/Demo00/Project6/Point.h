#pragma once
class Point
{
private:
	int x;
	int y;
public:
	Point(int x = 0, int y = 0);
	~Point();
	int getX();
	int getY();
	float getDist(Point &p);
};

