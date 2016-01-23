#ifndef __T05Coordinate_H__
#define __T05Coordinate_H__

#include "cocos2d.h"

USING_NS_CC;

class T05Coordinate : public CCLayer
{
public:
	static CCScene * scene();
	CREATE_FUNC(T05Coordinate);
	bool init();

	virtual void onEnter() override;
	virtual void onExit() override;
	virtual bool onTouchBegan(CCTouch *, CCEvent *) override;
	virtual void onTouchMoved(CCTouch *, CCEvent *) override;
	virtual void onTouchEnded(CCTouch *, CCEvent *) override;

protected:
	CCSprite *hero02_tx;
	CCPoint pOffset;
};

#endif
