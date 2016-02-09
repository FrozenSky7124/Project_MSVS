#ifndef __T21Particle_H__
#define __T21Particle_H__

#include "cocos2d.h"
#include "TBack.h"

USING_NS_CC;

class T21Particle :public TBack
{
public:
	static CCScene * scene();
	CREATE_FUNC(T21Particle);
	bool init();
};

#endif

