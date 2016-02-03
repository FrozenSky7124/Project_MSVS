#ifndef __T09Memory_H__
#define __T09Memory_H__

#include "cocos2d.h"
#include "TBack.h"

USING_NS_CC;

class T09Memory : public TBack
{
public:
	static CCScene * scene();
	CREATE_FUNC(T09Memory);
	bool init();

	//void mySchedule(float dt);
	void menuCallback(CCObject *obj);
protected:
	Sprite *spr;
	CCSprite *spr2;
	CCArray *array;
};

#endif