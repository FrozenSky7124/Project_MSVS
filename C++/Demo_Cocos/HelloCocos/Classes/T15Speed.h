#ifndef __T15Speed_H__
#define __T15Speed_H__

#include "cocos2d.h"
#include "TBack.h"

USING_NS_CC;

class T15Speed :public TBack
{
public:
	static CCScene * scene();
	CREATE_FUNC(T15Speed);
	bool init();

	void update(float dt);

	Sprite *hero0;
	Sprite *power;
	MoveBy *By1;
};

#endif