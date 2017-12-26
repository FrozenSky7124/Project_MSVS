#pragma once
class CPointSky : public POINT
{
public:
	CPointSky(int initX, int initY);
	CPointSky();
	~CPointSky();

	void Offset(int xOffset, int yOffset);
};

