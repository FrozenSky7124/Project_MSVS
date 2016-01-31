#ifndef __LayerOver_H__
#define __LayerOver_H__

#include "cocos2d.h"

USING_NS_CC;

class LayerOver : public Layer
{
public:
	static Scene * createScene(bool status);
	static LayerOver * create(bool status);
	bool init(bool status);

	//Function
	void menuCallBack(Object *obj);
};

#endif