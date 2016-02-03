#ifndef __TBack_H__
#define __TBack_H__

#include "cocos2d.h"

USING_NS_CC;

class TBack : public Layer
{
public:
	CREATE_FUNC(TBack);
	bool init();

	//Function
	void mainmenuCallBack(Ref *ref);
};

#endif