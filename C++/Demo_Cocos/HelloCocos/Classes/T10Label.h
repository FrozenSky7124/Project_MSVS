#ifndef __T10Label_H__
#define __T10Label_H__

#include "cocos2d.h"

USING_NS_CC;

class T10Label : public CCLayer
{
public:
	static CCScene * scene();
	CREATE_FUNC(T10Label);
	bool init();

	enum MyEnum
	{
		LTTF,LBM
	};

	void myTimeCount1(float dt);
	void myTimeCount2(float dt);
protected:

};

#endif