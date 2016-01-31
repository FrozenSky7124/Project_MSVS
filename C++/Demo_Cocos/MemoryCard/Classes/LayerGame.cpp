#include "LayerGame.h"
#include "LayerOver.h"
#include "CardItem.h"
#include <stdlib.h>
#include <time.h>
#include "SimpleAudioEngine.h"

#define winSize Director::getInstance()->getVisibleSize()

USING_NS_CC;

Scene* LayerGame::createScene(int level)
{
	Scene * scene = Scene::create();
	LayerGame *layer = LayerGame::create(level);
	scene->addChild(layer);
	return scene;
}

LayerGame* LayerGame::create(int level)
{
	LayerGame *pRet = new LayerGame;
	if (pRet && pRet->init(level))
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

bool LayerGame::init(int level)
{
	if (!Layer::init())
		return false;
	itime = 6;

	/*
	CardItem *item = CardItem::create(5);
	item->setAnchorPoint(ccp(0, 0));
	item->setPosition(ccp(80, 80));
	this->addChild(item);
	*/
	
	setclickStart(0);

	getAllPoints();
	getAvailablePoints(level);
	addCard(level);

	//Touch Function
	auto listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true);
	listener->onTouchBegan = CC_CALLBACK_2(LayerGame::onTouchBegan, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	//Audio
	CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect("click.wav");
	CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect("wrong.mp3");
	CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect("win.wav");

	//Update:AddSchedule
	CCString *strlv = CCString::createWithFormat("%d", level);


	schedule(schedule_selector(LayerGame::calltest), 1, 5, level-4);

	return true;
}

void LayerGame::getAllPoints()
{
	allPoints = CCPointArray::create(60);
	allPoints->retain();
	for (int i = 0;i < 10;i++)
	{
		for (int j = 0;j < 6;j++)
		{
			allPoints->addControlPoint(ccp(80 * i, 80 * j));
		}
	}
}

void LayerGame::getAvailablePoints(int level)
{
	srand(time(NULL));

	//Create
	while (true)
	{
		int idx = rand() % 60;
		auto itr = vec.begin();
		for (;itr != vec.end();++itr)
		{
			if (idx == *itr)
			{
				break;
			}
		}
		if (itr == vec.end())
		{
			vec.push_back(idx);
			if (vec.size() == level)
			{
				break;
			}
		}

	}
}

void LayerGame::addCard(int level)
{
	allCards = CCArray::create();
	allCards->retain();
	for (int i = 0;i < level;i++)
	{
		CardItem *item = CardItem::create(i);
		item->setPosition(CCPoint(allPoints->getControlPointAtIndex(vec[i])));
		this->addChild(item);
		item->showText();
		allCards->addObject(item);
	}
}

bool LayerGame::onTouchBegan(Touch *touch, Event* event)
{
	for (int i = 0;i < allCards->count();i++)
	{
		if(((CardItem*)allCards->getObjectAtIndex(i))->boundingBox().containsPoint(touch->getLocation()))
		{
			
			showCardAllWhite();
			unschedule(schedule_selector(LayerGame::calltest));
			itime = 6;
			Sprite *mytimeSpr = NULL;
			mytimeSpr = (Sprite *)this->getChildByTag(10001);
			if (mytimeSpr != NULL)
			{
				mytimeSpr->removeFromParent();
				mytimeSpr = NULL;
			}

			if(getclickStart() == i)
			{
				//_clickStart++;
				//Audio
				CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("click.wav");
				((CardItem *)allCards->getObjectAtIndex(i))->removeFromParentAndCleanup(true);
				allCards->removeObjectAtIndex(i);
			}
			else
			{
				//Audio
				CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("wrong.mp3");
				//MessageBoxA(0, "胜败乃兵家常事，大侠请重新再来！", "Warning", 1);
				Scene *scene = LayerOver::createScene(false);
				CCDirector::sharedDirector()->replaceScene(scene);
				CCLOG("You Lose!");
			}
			if(allCards->count()==0)
			{
				//MessageBoxA(0, "成功记忆！", "Warning", 1);
				CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("win.wav");
				Scene *scene = LayerOver::createScene(true);
				CCDirector::sharedDirector()->replaceScene(scene);
				CCLOG("You Win!");
			}
		}
	}
	return false;
}

void LayerGame::showCardAllWhite()
{
	for (int i = 0;i < allCards->count();i++)
	{
		CardItem *item = (CardItem*)allCards->getObjectAtIndex(i);
		item->showWhite();
	}
}

void LayerGame::calltest(float dt)
{
	
	CCLOG("xxxxxx %d", itime--);

	Sprite *mytimeSpr = NULL;
	mytimeSpr = (Sprite *)this->getChildByTag(10001);
	if (mytimeSpr != NULL)
	{
		mytimeSpr->removeFromParent();
		mytimeSpr = NULL;
	}
	
	switch (itime)
	{
	case 5:
		mytimeSpr = Sprite::create("5.png");
		break;
	case 4:
		mytimeSpr = Sprite::create("4.png");
		break;
	case 3:
		mytimeSpr = Sprite::create("3.png");
		break;
	case 2:
		mytimeSpr = Sprite::create("2.png");
		break;
	case 1:
		mytimeSpr = Sprite::create("1.png");
		break;
	case 0:
		mytimeSpr = Sprite::create("1.png");
		itime = 6;
		break;
	default:
		break;
	}
	mytimeSpr->setPosition(Point(winSize.width / 2, winSize.height / 2));
	mytimeSpr->setVisible(true);
	mytimeSpr->setTag(10001);
	this->addChild(mytimeSpr);

	if(itime == 6)
	{
		showCardAllWhite();
		unschedule(schedule_selector(LayerGame::calltest));
		mytimeSpr->removeFromParent();
		mytimeSpr = NULL;
	}
}