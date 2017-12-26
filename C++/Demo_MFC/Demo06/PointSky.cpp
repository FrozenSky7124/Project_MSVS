#include "stdafx.h"
#include "PointSky.h"

CPointSky::CPointSky(int initX, int initY)
{
	x = initX;
	y = initY;
}

CPointSky::CPointSky()
{
	x = 0;
	y = 0;
}

CPointSky::~CPointSky()
{
}

void CPointSky::Offset(int xOffset, int yOffset)
{
	x += xOffset;
	y += yOffset;
}