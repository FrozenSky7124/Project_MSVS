#include "T17ProgressTo.h"
#include "AppMarcos.h"

CCScene *T17ProgressTo::scene()
{
	CCScene * scene = CCScene::create();
	T17ProgressTo * layer = T17ProgressTo::create();
	scene->addChild(layer);
	return scene;
}
bool T17ProgressTo::init()
{
	TBack::init();

	Sprite *proBack = Sprite::create("loading_b_1.png");
	proBack->setPosition(ccp(winSize.width/2 - 100, winSize.height / 2));

	ProgressTimer *left = ProgressTimer::create(Sprite::create("loading_b_2.png"));
	left->setPosition(ccp(winSize.width/2 - 100, winSize.height / 2));
	left->setMidpoint(ccp(0, 0));
	left->setBarChangeRate(ccp(1, 0));
	left->setType(kCCProgressTimerTypeBar);
	//left->setType(kCCProgressTimerTypeRadial);

	addChild(proBack);
	addChild(left);

	ProgressTo *to = ProgressTo::create(2, 50.0f);
	left->runAction(RepeatForever::create(to));

	return true;
}