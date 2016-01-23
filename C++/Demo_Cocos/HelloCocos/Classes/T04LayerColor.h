#ifndef __T04LayerColor_H__
#define __T04LayerColor_H__

#include "cocos2d.h"

USING_NS_CC;

class T04LayerColor : public CCLayerColor
{
public:
	static CCScene * scene();
	CREATE_FUNC(T04LayerColor);
	bool init();
};

#endif