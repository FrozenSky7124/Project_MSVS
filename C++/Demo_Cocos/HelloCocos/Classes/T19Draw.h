#ifndef __T19Draw_H__
#define __T19Draw_H__

#include "cocos2d.h"
#include "TBack.h"

USING_NS_CC;

class T19Draw :public TBack
{
public:
	static CCScene * scene();
	CREATE_FUNC(T19Draw);
	bool init();

	virtual void draw(Renderer *renderer, const Mat4 &transform, uint32_t flags) override;
	void onDraw(const Mat4 &transform, uint32_t flags);

protected:
	CustomCommand _customCommand;
};

#endif