#ifndef __MapLayer_H__
#define __MapLayer_H__

#include "cocos2d.h"

USING_NS_CC;

class MapLayer : public LayerColor
{
public:

	virtual bool init();

	//CREATE_FUNC(MapLayer);
	static MapLayer * create(int level);

	int _level;
	int _score;
	CCArray *totalArray;
	CCArray *realArray;

	int getScore();
	int getLevel();

private:
	void initUI();
	void initMapNode();
	void debug();
};

#endif // __MapLayer_H__

