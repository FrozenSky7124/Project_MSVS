#ifndef __T08Touch_H__
#define __T08Touch_H__

#include "cocos2d.h"
#include "TBack.h"

USING_NS_CC;

class T08Touch : public TBack
{
public:
	static CCScene * scene();
	CREATE_FUNC(T08Touch);
	bool init();

	virtual bool onTouchBegan(CCTouch *touch, CCEvent *event) override;
	virtual void onTouchMoved(CCTouch *touch, CCEvent *event) override;
	virtual void onTouchEnded(CCTouch *touch, CCEvent *event) override;

protected:
	enum MyEnum
	{
		HelloWorld
	};
};

#endif

