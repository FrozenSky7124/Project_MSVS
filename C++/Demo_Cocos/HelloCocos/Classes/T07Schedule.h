#ifndef __T07Schedule_H__
#define __T07Schedule_H__

#include "cocos2d.h"
#include "TBack.h"

USING_NS_CC;

class T07Schedule : public TBack
{
public:
	static CCScene * scene();
	CREATE_FUNC(T07Schedule);
	bool init();

	virtual void update(float dt) override;

	void mySchedule(float dt);
	void heroMove(float dt);
};

#endif