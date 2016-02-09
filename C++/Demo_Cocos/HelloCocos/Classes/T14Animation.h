#ifndef __T14Animation_H__
#define __T14Animation_H__

#include "cocos2d.h"
#include "TBack.h"

USING_NS_CC;

class T14Animation :public TBack
{
public:
	static CCScene * scene();
	CREATE_FUNC(T14Animation);
	bool init();

	void onEnter();
	void onEnterTransitionDidFinish();
};

#endif