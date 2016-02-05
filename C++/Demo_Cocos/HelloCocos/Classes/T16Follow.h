#ifndef __T16Follow_H__
#define __T16Follow_H__

#include "cocos2d.h"
#include "TBack.h"

USING_NS_CC;

class T16Follow :public TBack
{
public:
	static CCScene * scene();
	CREATE_FUNC(T16Follow);
	bool init();

	void funcNCallBack(Node *node);

	Sprite *hero0;
	MoveBy *By1;
};

#endif