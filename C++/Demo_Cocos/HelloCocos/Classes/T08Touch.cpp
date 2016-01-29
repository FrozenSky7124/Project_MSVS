#include "T08Touch.h"
#include "AppMarcos.h"

CCScene * T08Touch::scene()
{
	CCScene *scene = CCScene::create();
	T08Touch *layer = T08Touch::create();
	scene->addChild(layer);
	return scene;
}

bool T08Touch::init()
{
	CCLayer::init();

	CCSprite *spr = CCSprite::create("HelloWorld.png");
	spr->setPosition(ccp(winSize.width / 2, winSize.height / 2));
	spr->setTag(HelloWorld);
	this->addChild(spr);

	auto listener = EventListenerTouchOneByOne::create();
	//设置吞噬：不向下传递触摸
	listener->setSwallowTouches(true);
	listener->onTouchBegan = CC_CALLBACK_2(T08Touch::onTouchBegan, this);
	listener->onTouchMoved = CC_CALLBACK_2(T08Touch::onTouchMoved, this);
	listener->onTouchEnded = CC_CALLBACK_2(T08Touch::onTouchEnded, this);

	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);


	return true;
}

bool T08Touch::onTouchBegan(CCTouch *touch, CCEvent *event)
{
	CCLOG("onTouchBegan!");
	CCSprite *spr = (CCSprite*)getChildByTag(HelloWorld);
	if (spr->boundingBox().containsPoint(touch->getLocation()))
	{
		return true;
	}
	return false;
}

void T08Touch::onTouchMoved(CCTouch *touch, CCEvent *event)
{
	CCSprite *spr = (CCSprite*)getChildByTag(HelloWorld);
	CCPoint pt = spr->getPosition() + touch->getDelta();
	if ((pt.x <= (winSize.width - spr->getContentSize().width / 2))
		&& (pt.x >= (spr->getContentSize().width / 2))
		&& (pt.y <= (winSize.height - spr->getContentSize().height / 2))
		&& (pt.y >= (spr->getContentSize().height / 2)))
	{
		spr->setPosition(pt);
	}
}

void T08Touch::onTouchEnded(CCTouch *touch, CCEvent *event)
{

}