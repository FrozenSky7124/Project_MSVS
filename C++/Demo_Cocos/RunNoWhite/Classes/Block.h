#ifndef __Block_H__
#define __Block_H__

#include "cocos2d.h"

USING_NS_CC;

class Block : public Sprite
{
public:
	static Block* create(Size size, Color3B color,
						 CCString str, float strSize, Color3B strColor);
	bool init(Size size, Color3B color,
			  CCString str, float strSize, Color3B strColor);

	static CCArray * array;
	static CCArray * getBlockArray();

	void moveDownAndCleanUp();

	CC_SYNTHESIZE(int, _lineIndex, LineIndex)
};

#endif