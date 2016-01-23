#ifndef __T01LayerAnchorPoint_H__
#define __T01LayerAnchorPoint_H__

#include "cocos2d.h"

USING_NS_CC;

class T01LayerAnchorPoint : public CCLayer
{
public:
	static T01LayerAnchorPoint * create();
	bool init();
	
	static CCScene * scene();

	virtual void draw(Renderer *renderer, const Mat4 &transform, uint32_t flags) override;
	void onDraw(const Mat4 &transform, uint32_t flags);

	void mySchedule(float dt);

protected:
	CustomCommand _customCommand;
	CCSprite *spr;
};

#endif

