#ifndef __T02LayerSprite_H__
#define __T02LayerSprite_H__

#include "cocos2d.h"

USING_NS_CC;

class T02LayerSprite : public CCLayer
{
public:
	static CCScene * scene();
	CREATE_FUNC(T02LayerSprite);
	bool init();
};

#endif