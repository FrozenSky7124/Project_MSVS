#include "T02LayerSprite.h"
#include "AppMarcos.h"

CCScene * T02LayerSprite::scene()
{
	CCScene *scene = CCScene::create();
	T02LayerSprite *layer = T02LayerSprite::create();
	scene->addChild(layer);
	return scene;
}

bool T02LayerSprite::init()
{
	if (!CCLayer::init())
		return false;

	//CCSprite *bg = CCSprite::create("bg_001.jpg");
	CCSprite *bg = CCSprite::create("bg_001.jpg", CCRectMake(0, 0, 1024, 769));
	bg->setAnchorPoint(ccp(0, 0));
	bg->setPosition(ccp(0, 0));
	this->addChild(bg);
	
	CCImage *image = new CCImage();
	image->autorelease();
	image->initWithImageFile("hero2_001.png");
	CCTexture2D *texture_hero02 = new CCTexture2D();
	texture_hero02->autorelease();
	texture_hero02->initWithImage(image);
	CCSprite *hero02 = CCSprite::createWithTexture(texture_hero02);
	hero02->setPosition(ccp(winSize.width / 2, winSize.height / 2));
	this->addChild(hero02);

	CCTexture2D *texture_hero01 = CCTextureCache::sharedTextureCache()->addImage("hero1_001.png");
	CCSprite *hero01 = CCSprite::createWithTexture(texture_hero01);
	hero01->setPosition(ccp(winSize.width / 2 + 70, winSize.height / 2));
	this->addChild(hero01);

	CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("hero0_move.plist");
	//CCSpriteFrame *sf = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("hero0_01.png");
	//CCSprite *hero00 = CCSprite::createWithSpriteFrame(sf);
	CCSprite *hero00 = CCSprite::createWithSpriteFrameName("hero0_01.png");
	hero00->setPosition(ccp(winSize.width / 2 + 180, winSize.height / 2));
	this->addChild(hero00);

	//ÌøÔ¾¶¯×÷
	//CCJumpBy *jump = CCJumpBy::create(2, ccp(0, 0), 40, 3);
	//hero00->runAction(CCRepeatForever::create(jump));

	return true;
}