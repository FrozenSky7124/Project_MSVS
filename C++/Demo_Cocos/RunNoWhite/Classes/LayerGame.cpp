#include "LayerGame.h"
#include "AppMacros.h"
#include "Block.h"
#include <time.h>

Scene* LayerGame::createScene()
{
	auto scene = Scene::create();
	auto layer = LayerGame::create();
	scene->addChild(layer);
	return scene;
}

bool LayerGame::init()
{
	if (!Layer::init())
	{
		return false;
	}

	startGame();
	setLineCount(0);
	showEnd = false;
	isRunning = false;
	startTime = 0;
	ttf = LabelTTF::create("0.0000", "Courier New", 40);
	ttf->setZOrder(100);
	ttf->setPosition(Point(winSize.width / 2, winSize.height - 40));
	ttf->setColor(Color3B::BLUE);
	addChild(ttf);

	auto listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true);
	listener->onTouchBegan = CC_CALLBACK_2(LayerGame::onTouchBegan, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	return true;
}

void LayerGame::startGame()
{
	addStartLineBlocks();
	addNormalLineBlocks(1);
	addNormalLineBlocks(2);
	addNormalLineBlocks(3);
}

void LayerGame::addStartLineBlocks()
{
	Size startBlockSize = Size(winSize.width, winSize.height / 4);
	Block *b = Block::create(startBlockSize, Color3B::YELLOW,
							 "Start Game", 30, Color3B::BLACK);
	b->setPosition(Point(0, 0));
	b->setLineIndex(0);
	addChild(b);
	_lineCount++;
}

void LayerGame::addNormalLineBlocks(int lineIndex)
{
	Size normalBlockSize = Size(winSize.width / 4 - 1, winSize.height / 4 - 1);
	srand(time(0));
	int idx = rand() % 4;
	for (int i = 0; i < 4; i++)
	{
		Block *b = Block::create(normalBlockSize,
								 i == idx ? Color3B::BLACK : Color3B::WHITE,
								 "",
								 30,
								 Color3B::WHITE);
		b->setLineIndex(lineIndex);
		b->setPosition(Point(i*winSize.width / 4, lineIndex*winSize.height / 4));
		addChild(b);
	}
	_lineCount++;
}

bool LayerGame::onTouchBegan(Touch *touch, Event *event)
{
	CCArray *array = Block::getBlockArray();
	for (int i = 0; i < array->count(); i++)
	{
		Block *b = (Block*)array->getObjectAtIndex(i);
		if (b->boundingBox().containsPoint(touch->getLocation()) && b->getLineIndex() == 1)
		{
			if(b->getColor() == Color3B::BLACK)
			{
				CCLOG("Click Black. lineCount = %d", getLineCount());
				startTimer();
				b->setColor(Color3B::GRAY);
				this->moveDown();
			}
			else if(b->getColor() == Color3B::GREEN)
			{
				CCLOG("Click Green. lineCount = %d", getLineCount());
				stopTimer();
				this->moveDown();
			}
			else if(b->getColor() == Color3B::WHITE)
			{
				//GameOver Function
				CCLOG("Click White. lineCount = %d", getLineCount());
			}
			break;
		}
	}
	return false;
}

void LayerGame::moveDown()
{
	if (getLineCount() < 10)
	{
		addNormalLineBlocks(4);
	}
	else if(!showEnd)
	{
		addEndLineBlocks();
		showEnd = true;
	}

	CCArray *array = Block::getBlockArray();

	for (int i = array->count() - 1; i >= 0; i--)
	{
		Block *b = (Block*)array->getObjectAtIndex(i);
		b->moveDownAndCleanUp();
	}
}

void LayerGame::addEndLineBlocks()
{
	Block *b = Block::create(winSize, Color3B::GREEN, "Game Over", 30, Color3B::BLACK);
	b->setAnchorPoint(Point(0, 0));
	b->setPosition(Point(0, winSize.height/4*4));
	addChild(b);
	b->setLineIndex(4);
	_lineCount++;
}

void LayerGame::startTimer()
{
	if(!isRunning)
	{
		scheduleUpdate();
		startTime = clock();
		isRunning = true;
	}
}

void LayerGame::stopTimer()
{
	if(isRunning)
	{
		unscheduleUpdate();
	}
}

void LayerGame::update(float delta)
{
	long long offset = clock() - startTime;
	CCString *str = CCString::createWithFormat("%.2g",
											   (double)offset / 1000);
	ttf->setString(str->getCString());
}