#ifndef __T20Music_H__
#define __T20Music_H__

#include "cocos2d.h"
#include "TBack.h"

USING_NS_CC;

class T20Music :public TBack
{
public:
	static CCScene * scene();
	CREATE_FUNC(T20Music);
	bool init();

	bool onTouchBegan(Touch *touch, Event *unused_event);
};

#endif