#ifndef __LayerStart_H__
#define __LayerStart_H__

#include "cocos2d.h"

USING_NS_CC;

class LayerStart : public Layer
{
public:
	static Scene* createScene();

	virtual bool init();

	CREATE_FUNC(LayerStart);

	//Attribute
	enum LEVEL
	{
		EASY=5,HARD=7,VERYHARD=9
	};

	//Function
	void easy(CCObject *obj);
	void hard(CCObject *obj);
	void very(CCObject *obj);
	void quit(CCObject *obj);
};

#endif