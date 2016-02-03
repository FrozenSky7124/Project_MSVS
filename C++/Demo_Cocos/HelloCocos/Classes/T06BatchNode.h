#ifndef __T06BatchNode_H__
#define __T06BatchNode_H__

#include "cocos2d.h"
#include "TBack.h"

USING_NS_CC;

class T06BatchNode : public TBack
{
public:
	static CCScene * scene();
	CREATE_FUNC(T06BatchNode);
	bool init();

	virtual void onEnter() override;
	virtual void onExit() override;

	bool onTouchDemo1(CCTouch *touch, CCEvent *event);

protected:
	CCSpriteBatchNode *batchNode;
};

#endif
