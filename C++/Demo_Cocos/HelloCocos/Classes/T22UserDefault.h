#ifndef __T22UserDefault_H__
#define __T22UserDefault_H__

#include "cocos2d.h"
#include "TBack.h"

USING_NS_CC;

class T22UserDefault :public TBack
{
public:
	static CCScene * scene();
	CREATE_FUNC(T22UserDefault);
	bool init();
};

#endif

