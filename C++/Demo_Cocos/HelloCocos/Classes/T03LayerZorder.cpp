#include "T03LayerZorder.h"
#include "AppMarcos.h"

CCScene * T03LayerZorder::scene()
{
	CCScene *scene = CCScene::create();
	T03LayerZorder *layer = T03LayerZorder::create();
	scene->addChild(layer);
	return scene;
}

bool T03LayerZorder::init()
{
	if (!CCLayer::init())
		return false;

	CCSprite * hero1_tx = CCSprite::create("hero1_tx.png");
	CCSprite * hero2_tx = CCSprite::create("hero2_tx.png");
	CCSprite * hero3_tx = CCSprite::create("hero3_tx.png");

	hero1_tx->setPosition(ccp(winSize.width / 2 - 70, winSize.height / 2));
	hero2_tx->setPosition(ccp(winSize.width / 2, winSize.height / 2));
	hero3_tx->setPosition(ccp(winSize.width / 2 + 90, winSize.height / 2 - 30));

	hero1_tx->setZOrder(10);
	hero2_tx->setZOrder(5);
	hero3_tx->setZOrder(0);

	this->addChild(hero1_tx);
	this->addChild(hero2_tx);
	this->addChild(hero3_tx);

	CCLOG("hero1_tx.Zorder=%d", hero1_tx->getZOrder());
	CCLOG("hero2_tx.Zorder=%d", hero2_tx->getZOrder());
	CCLOG("hero3_tx.Zorder=%d", hero3_tx->getZOrder());

	return true;
}