#ifndef __T18Grid3D_H__
#define __T18Grid3D_H__

#include "cocos2d.h"
#include "TBack.h"

USING_NS_CC;

class T18Grid3D :public TBack
{
public:
	static CCScene * scene();
	CREATE_FUNC(T18Grid3D);
	bool init();
};

#endif
