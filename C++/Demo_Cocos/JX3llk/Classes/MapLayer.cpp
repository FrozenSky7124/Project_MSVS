#include "MapLayer.h"
#include "AppMacro.h"
#include "MapNode.h"

#define MapRow 6
#define MapColumn 12

bool MapLayer::init()
{
	if (!LayerColor::init())
	{
		return false;
	}

	initUI();
	initMapNode();
	debug();

	return true;
}

MapLayer * MapLayer::create(int level)
{
	MapLayer *pRet = new MapLayer;
	if(pRet && pRet->init())
	{
		pRet->autorelease();
		pRet->_level = level;
	}
	else
	{
		delete pRet;
		pRet = NULL;
	}
	return pRet;
}

void MapLayer::initUI()
{
	this->setAnchorPoint(Point(0, 0));
	this->setContentSize(Size(70 * MapColumn, 70 * MapRow));
	this->setPosition(Point(winSize.width / 2 - this->getContentSize().width / 2, 70));
	//this->setColor(Color3B::BLACK);
	//this->setOpacity(50);

	this->_score = 000;
}

void MapLayer::initMapNode()
{
	totalArray = CCArray::create();
	realArray = CCArray::create();
	int totalNum = MapRow*MapColumn;
	CCArray *tempArray = CCArray::create();
	for (int i = 0; i < totalNum / 2; i++)
	{
		int index = CCRANDOM_0_1() * 7 + 1;
		CCLOG("%d", index);
		MapNode *tempNode1 = MapNode::create(index);
		MapNode *tempNode2 = MapNode::create(index);
		tempArray->addObject(tempNode1);
		tempArray->addObject(tempNode2);
	}

	for (int i = 0; i < MapRow + 2; i++)
	{
		for (int j = 0; j < MapColumn + 2; j++)
		{
			if (i == 0 || j == 0 || i == MapRow + 1 || j == MapColumn + 1)
			{
				MapNode *temp = MapNode::create(0);
				temp->setX(j);
				temp->setY(i);
				temp->setAnchorPoint(Vec2(0, 0));
				temp->setPosition(Vec2(70 * (j - 1), 70 * (i - 1)));
				this->addChild(temp);
				this->totalArray->addObject(temp);
			}
			else
			{
				MapNode *temp = (MapNode*)tempArray->getRandomObject();
				tempArray->removeObject(temp, false);
				temp->setX(j);
				temp->setY(i);
				temp->setAnchorPoint(Vec2(0, 0));
				temp->setPosition(Vec2(70 * (j - 1), 70 * (i - 1)));
				this->addChild(temp);
				this->totalArray->addObject(temp);
				this->realArray->addObject(temp);
			}
		}
	}
}

int MapLayer::getLevel()
{
	return this->_level;
}

int MapLayer::getScore()
{
	return this->_score;
}

void MapLayer::debug()
{
	CCLOG("MapSize x=%g y=%g", this->getContentSize().width, this->getContentSize().height);
	//this->_score = 777;
}