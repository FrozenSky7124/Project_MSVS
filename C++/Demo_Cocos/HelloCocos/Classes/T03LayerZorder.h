#ifndef __T03LayerZorder_H__
#define __T03LayerZorder_H__

#include "cocos2d.h"

USING_NS_CC;

class T03LayerZorder : public CCLayer
{
public:
	static CCScene * scene();
	CREATE_FUNC(T03LayerZorder);
	bool init();
};

#endif