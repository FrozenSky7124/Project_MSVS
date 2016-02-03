#ifndef __T11CCMenu_H__
#define __T11CCMenu_H__

#include "cocos2d.h"
#include "TBack.h"

USING_NS_CC;

class T11CCMenu : public TBack
{
public:
	static CCScene * scene();
	CREATE_FUNC(T11CCMenu);
	bool init();

	//Function
	void menuItem1_callback(Ref *ref);

};

#endif