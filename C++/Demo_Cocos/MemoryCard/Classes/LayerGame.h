#ifndef __LayerGame_H__
#define __LayerGame_H__

#include "cocos2d.h"
#include <vector>

USING_NS_CC;

class LayerGame : public Layer
{
public:
	static Scene* createScene(int level);
	static LayerGame* create(int level);
	bool init(int level);

	//void onEnter();
	//void onExit();

	bool onTouchBegan(Touch *touch, Event* event);
	//void onTouchMoved(Touch *touch, Event* event);
	//void onTouchEnded(Touch *touch, Event* event);

	//Attribute
	CCPointArray *allPoints;
	CCArray *allCards;
	std::vector<int> vec;
	int itime;

	//Function
	void getAllPoints();
	void getAvailablePoints(int level);
	void addCard(int level);
	void showCardAllWhite();
	void calltest(float dt);

	CC_SYNTHESIZE(int, _clickStart, clickStart)
};

#endif