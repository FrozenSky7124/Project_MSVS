#ifndef __T12UI_H__
#define __T12UI_H__

#include "cocos2d.h"
#include "TBack.h"
#include "extensions/cocos-ext.h"

USING_NS_CC;
USING_NS_CC_EXT;

class T12UI :public TBack
{
public:
	static CCScene * scene();
	CREATE_FUNC(T12UI);
	bool init();

	LabelTTF *atlas;

	void sliderCallBack(Ref* sender, Control::EventType event);
	void switchCallBack(Ref* sender, Control::EventType event);
};


#endif