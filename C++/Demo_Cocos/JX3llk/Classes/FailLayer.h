#ifndef __FailLayer_H__
#define __FailLayer_H__

#include "cocos2d.h"

USING_NS_CC;

class FailLayer : public Layer
{
public:
	static Scene* createScene();

	virtual bool init();

	CREATE_FUNC(FailLayer);

	void initUI();

	void quitGameCallBack(Ref *ref);
};

#endif // __FailLayer_H__
