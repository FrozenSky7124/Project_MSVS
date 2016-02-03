#ifndef __TMenu_H__
#define __TMenu_H__

#include "cocos2d.h"

USING_NS_CC;

class TMenu : public Layer
{
public:
	static Scene* createScene();
	CREATE_FUNC(TMenu);
	bool init();

	//Function
	void menuCallBack(Ref* ref);
	void closeCallBack(Ref* ref);

	bool onTouchBegan(Touch *touch, Event *event);
	void onTouchMoved(Touch *touch, Event *event);
};
#endif
