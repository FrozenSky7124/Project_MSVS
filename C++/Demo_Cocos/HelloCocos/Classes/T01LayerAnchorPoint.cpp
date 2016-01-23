#include "T01LayerAnchorPoint.h"

T01LayerAnchorPoint * T01LayerAnchorPoint::create()
{
	T01LayerAnchorPoint * pRet = new T01LayerAnchorPoint();
	if (pRet && pRet->init())
	{
		pRet->autorelease();
	}
	else
	{
		delete pRet;
		pRet = NULL;
	}
	return pRet;
}

bool T01LayerAnchorPoint::init()
{
	if(!CCLayer::init())
		return false;

	CCSize winSize = CCDirector::sharedDirector()->getWinSize();
	spr = CCSprite::create("hero2_001.png");
	//spr->ignoreAnchorPointForPosition(true);
	spr->setScale(1.0);
	spr->setPosition(ccp(winSize.width / 2, winSize.height / 2));

	this->addChild(spr);

	schedule(schedule_selector(T01LayerAnchorPoint::mySchedule), 0.1);
	return true;
}

CCScene * T01LayerAnchorPoint::scene()
{
	CCScene * scene = CCScene::create();
	T01LayerAnchorPoint * layer = T01LayerAnchorPoint::create();
	scene->addChild(layer);
	return scene;
}

//重写cocos2d-x 3.x draw方法：回调onDraw函数进行OpenGL绘图
void T01LayerAnchorPoint::draw(Renderer *renderer, const Mat4 &transform, uint32_t flags)
{
	_customCommand.init(_globalZOrder, transform, flags);
	_customCommand.func = CC_CALLBACK_0(T01LayerAnchorPoint::onDraw, this, transform, flags);
	renderer->addCommand(&_customCommand);
}

void T01LayerAnchorPoint::onDraw(const Mat4 &transform, uint32_t flags)
{
	CCSize winSize = CCDirector::sharedDirector()->getWinSize();
	ccDrawColor4B(255, 0, 0, 255);
	ccDrawLine(ccp(0, winSize.height / 2), ccp(winSize.width, winSize.height / 2));
	ccDrawLine(ccp(winSize.width / 2, 0), ccp(winSize.width / 2, winSize.height));
}

void T01LayerAnchorPoint::mySchedule(float dt)
{
	static float ro = 0;
	ro += 10;
	spr->setRotation(ro);
}
