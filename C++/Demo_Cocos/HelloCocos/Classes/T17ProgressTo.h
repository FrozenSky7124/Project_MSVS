#ifndef __T17ProgressTo_H__
#define __T17ProgressTo_H__

#include "cocos2d.h"
#include "TBack.h"

USING_NS_CC;

class T17ProgressTo :public TBack
{
public:
	static CCScene * scene();
	CREATE_FUNC(T17ProgressTo);
	bool init();
};

#endif