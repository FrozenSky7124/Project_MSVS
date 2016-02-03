#include "T06BatchNode.h"
#include "AppMarcos.h"

CCScene * T06BatchNode::scene()
{
	CCScene *scene = CCScene::create();
	T06BatchNode *layer = T06BatchNode::create();
	scene->addChild(layer);
	return scene;
}

bool T06BatchNode::init()
{
	TBack::init();
	/* Demo1
	batchNode = CCSpriteBatchNode::create("hero2_001.png");
	*/
	
	CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("hero0_move.plist", "hero0_move.png");
	batchNode = CCSpriteBatchNode::create("hero0_move.png");
	

	for (int i = 0;i < 1000;i++)
	{
		Sprite *spr = Sprite::create("HelloWorld.png");
		spr->setPosition(ccp(CCRANDOM_0_1() * 1024, CCRANDOM_0_1() * 768));
		this->addChild(spr);
	}
	this->addChild(batchNode);

	return true;
}

void T06BatchNode::onEnter()
{
	CCLayer::onEnter();

	// Register Touch Event
	auto listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true);

	listener->onTouchBegan = CC_CALLBACK_2(T06BatchNode::onTouchDemo1, this);

	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}

void T06BatchNode::onExit()
{
	CCLayer::onExit();
}

bool T06BatchNode::onTouchDemo1(CCTouch *touch, CCEvent *event)
{
	CCSprite *spr;
	for (int i = 0;i < 1000;i++)
	{
		/* Demo1
		CCSprite *spr = CCSprite::create("hero2_001.png");
		spr->setPosition(ccp(CCRANDOM_0_1() * 1024, CCRANDOM_0_1() * 768));
		batchNode->addChild(spr);
		*/

		int n = (int)(CCRANDOM_0_1() * 10);
		switch (n)
		{
		case 0:
			spr = CCSprite::createWithSpriteFrameName("hero0_01.png");
			break;
		case 1:
			spr = CCSprite::createWithSpriteFrameName("hero0_02.png");
			break;
		case 2:
			spr = CCSprite::createWithSpriteFrameName("hero0_03.png");
			break;
		case 3:
			spr = CCSprite::createWithSpriteFrameName("hero0_04.png");
			break;
		case 4:
			spr = CCSprite::createWithSpriteFrameName("hero0_05.png");
			break;
		case 5:
			spr = CCSprite::createWithSpriteFrameName("hero0_06.png");
			break;
		case 6:
			spr = CCSprite::createWithSpriteFrameName("hero0_07.png");
			break;
		case 7:
			spr = CCSprite::createWithSpriteFrameName("hero0_08.png");
			break;
		case 8:
			spr = CCSprite::createWithSpriteFrameName("hero0_09.png");
			break;
		case 9:
			spr = CCSprite::createWithSpriteFrameName("hero0_10.png");
			break;
		default:
			break;
		}
		spr->setPosition(ccp(CCRANDOM_0_1() * 1024, CCRANDOM_0_1() * 768));
		batchNode->addChild(spr);
	}
	return true;
}