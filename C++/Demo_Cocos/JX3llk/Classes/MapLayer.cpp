#include "MapLayer.h"
#include "AppMacro.h"
#include "MapNode.h"
#include "SimpleAudioEngine.h"
#include <stdlib.h>
#include <time.h>

#define MapRow 6
#define MapColumn 12
#define ImgNum 7

bool MapLayer::init(int level)
{
	if (!LayerColor::init())
	{
		return false;
	}

	initUI();
	initMapNode(level);
	initTouch();
	debug();

	return true;
}

MapLayer * MapLayer::create(int level, int score)
{
	MapLayer *pRet = new MapLayer;
	if (pRet && pRet->init(level))
	{
		pRet->autorelease();
		pRet->_level = level;
		pRet->_score = score;
		pRet->_remainNum = MapRow*MapColumn;
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
	 
	CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect("Sound/biubiubiu.wav");

	this->_preNode = NULL;
	this->_curNode = NULL;
}

void MapLayer::initMapNode(int templevel)
{
	int imageNum = 6;
	if (templevel == 1)
	{
		imageNum = 6;
	}
	else if (templevel == 2)
	{
		imageNum = 7;
	}
	else if (templevel == 3)
	{
		imageNum = 8;
	}
	else if (templevel == 4)
	{
		imageNum = 9;
	}
	else if (templevel == 5)
	{
		imageNum = 10;
	}
	else if (templevel > 5)
	{
		imageNum = 10;
	}
	CCLOG("imageNum=%d",imageNum);
	CCLOG("this->_level=%d", this->_level);
	totalArray = CCArray::create();
	totalArray->retain();
	realArray = CCArray::create();
	realArray->retain();
	int totalNum = MapRow*MapColumn;
	CCArray *tempArray = CCArray::create();
	srand(time(NULL));
	for (int i = 0; i < totalNum / 2; i++)
	{
		//int index = CCRANDOM_0_1() * 7 + 1;
		int index = rand() % imageNum + 1;
		//CCLOG("%d", index);
		MapNode *tempNode1 = MapNode::create(index);
		MapNode *tempNode2 = MapNode::create(index);
		tempArray->addObject(tempNode1);
		tempArray->addObject(tempNode2);
	}

	//构建关卡地图：周围块id = -1
	for (int i = 0; i < MapRow + 2; i++)
	{
		for (int j = 0; j < MapColumn + 2; j++)
		{
			if (i == 0 || j == 0 || i == MapRow + 1 || j == MapColumn + 1)
			{
				MapNode *temp = MapNode::create(-1);
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

void MapLayer::initTouch()
{
	auto listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(false);
	listener->onTouchBegan = CC_CALLBACK_2(MapLayer::onTouchBegan, this);
	listener->onTouchEnded = CC_CALLBACK_2(MapLayer::onTouchEnded, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}

bool MapLayer::onTouchBegan(Touch *touch, Event *event)
{
	if (this->getBoundingBox().containsPoint(touch->getLocation()))
	{
		//击中MapLayer范围
		return true;
	}

	return false;
}

void MapLayer::onTouchEnded(Touch *touch, Event *event)
{
	if (_preNode == NULL)
	{
		onMapNode(&_preNode, touch);
		_preNode->onSelect();
		return;
	}
	else if (_preNode == onMapNode(&_curNode, touch))
	{
		//_preNode与_curNode相同，选择了同一个点
		_preNode->offSelect();
		CCLOG("_preNode == _curNode");
		_preNode = NULL;
		_curNode = NULL;
		return;
	}
	else
	{
		//_preNode与_curNode不同，选择了两个点
		_curNode->onSelect();
		if (isSameIndex(_preNode, _curNode) && match(_preNode, _curNode) == true)
		{
			//可以消除
			clearNode(_preNode, _curNode);
		}
		else
		{
			//不可以消除
			_preNode->offSelect();
			_preNode = _curNode;
			_curNode = NULL;
		}
	}
}

MapNode * MapLayer::onMapNode(MapNode** temp, Touch *touch)
{
	for (int i = 0; i < realArray->count(); i++)
	{
		MapNode *tempNode = (MapNode*)realArray->getObjectAtIndex(i);

		if (tempNode->getBoundingBox().containsPoint(this->convertToNodeSpace(touch->getLocation())))
		{
			//击中MapNode
			*temp = tempNode;
			//(*temp)->runBlink();
			CCLOG("_preNode x=%d y=%d", (*temp)->getX(), (*temp)->getY());
		}
	}
	return *temp;
}

bool MapLayer::isSameIndex(MapNode *pre, MapNode *cur)
{
	if ((pre->getIndex() == cur->getIndex()) && (pre->getIndex() != -1) && (cur->getIndex() != -1))
	{
		return true;
	}
	return false;
}

MapNode * MapLayer::PointToMapNode(int x, int y)
{
	int num = y * (MapColumn + 2) + x;
	return (MapNode *)totalArray->getObjectAtIndex(num);
}

//判断是否可连通
bool MapLayer::match(MapNode *pre, MapNode *cur)
{
	if (matchDirect(pre, cur) == true)
	{
		return true;
	}
	if (matchOnecorner(pre, cur) == true)
	{
		return true;
	}
	if (matchTwocorner(pre, cur) == true)
	{
		return true;
	}
	return false;
}

//直线连通判断函数
bool MapLayer::matchDirect(MapNode *pre, MapNode *cur)
{
	if (!((pre->getX() == cur->getX()) || (pre->getY() == cur->getY())))
	{
		return false;
	}
	CCLOG("Into matchDirect Function!");
	//判断横向连通
	bool match_x = false;
	if (pre->getY() == cur->getY())
	{
		match_x = true;

		if (pre->getX() > cur->getX())
		{
			for (int i = cur->getX() + 1; i < pre->getX(); i++)
			{
				int num = cur->getY()*(MapColumn + 2) + i;
				MapNode *temp = (MapNode *)totalArray->getObjectAtIndex(num);
				if (temp->getIndex() != -1)
				{
					match_x = false;
				}
			}
		}

		if (pre->getX() < cur->getX())
		{
			for (int i = pre->getX() + 1; i < cur->getX(); i++)
			{
				int num = cur->getY()*(MapColumn + 2) + i;
				MapNode *temp = (MapNode *)totalArray->getObjectAtIndex(num);
				if (temp->getIndex() != -1)
				{
					match_x = false;
				}
			}
		}
	}

	//判断纵向连通
	bool match_y = false;
	if (pre->getX() == cur->getX())
	{
		match_y = true;

		if (pre->getY() > cur->getY())
		{
			for (int i = cur->getY() + 1; i < pre->getY(); i++)
			{
				int num = i * (MapColumn + 2) + cur->getX();
				MapNode *temp = (MapNode*)totalArray->getObjectAtIndex(num);
				if (temp->getIndex() != -1)
				{
					match_y = false;
				}
			}
		}

		if (pre->getY() < cur->getY())
		{
			for (int i = pre->getY() + 1; i < cur->getY(); i++)
			{
				int num = i * (MapColumn + 2) + cur->getX();
				MapNode *temp = (MapNode*)totalArray->getObjectAtIndex(num);
				if (temp->getIndex() != -1)
				{
					match_y = false;
				}
			}
		}
	}

	return match_x || match_y;
}

//有1个折点的连通判断函数
bool MapLayer::matchOnecorner(MapNode *pre, MapNode *cur)
{
	//获得折点
	int x1 = pre->getX();
	int y1 = cur->getY();
	int x2 = cur->getX();
	int y2 = pre->getY();

	MapNode *temp1 = (MapNode *)totalArray->getObjectAtIndex(y1 * (MapColumn + 2) + x1);
	MapNode *temp2 = (MapNode *)totalArray->getObjectAtIndex(y2 * (MapColumn + 2) + x2);

	if ((temp1->getIndex() == -1) && (matchDirect(pre, temp1)) && (matchDirect(cur, temp1)))
	{
		return true;
	}

	if ((temp2->getIndex() == -1) && (matchDirect(pre, temp2)) && (matchDirect(cur, temp2)))
	{
		return true;
	}

	return false;
}

//有2个折点的连通判断函数
bool MapLayer::matchTwocorner(MapNode *pre, MapNode *cur)
{
	int x = pre->getX();
	int y = pre->getY();
	
	//Left寻路
	for (int i = x - 1; i >= 0; i--)
	{
		if (!(PointToMapNode(i, y)->getIndex() == -1))
		{
			break;
		}
		if (matchOnecorner(PointToMapNode(i, y), cur))
		{
			return true;
		}
	}
	//Right寻路
	for (int i = x + 1; i <= MapColumn + 1; i++)
	{
		if (!(PointToMapNode(i, y)->getIndex() == -1))
		{
			break;
		}
		if (matchOnecorner(PointToMapNode(i, y), cur))
		{
			return true;
		}
	}
	//Top寻路
	for (int i = y + 1; i <= MapRow + 1; i++)
	{
		if (!(PointToMapNode(x, i)->getIndex() == -1))
		{
			break;
		}
		if (matchOnecorner(PointToMapNode(x, i), cur))
		{
			return true;
		}
	}
	//Bottom寻路
	for (int i = y - 1; i >= 0; i--)
	{
		if (!(PointToMapNode(x, i)->getIndex() == -1))
		{
			break;
		}
		if (matchOnecorner(PointToMapNode(x, i), cur))
		{
			return true;
		}
	}
	return false;
}

//清除图块：图块Index = -1
void MapLayer::clearNode(MapNode *pre, MapNode *cur)
{
	CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("Sound/biubiubiu.wav");
	this->_score++;
	this->_remainNum = this->_remainNum - 2;
	pre->setIndex(-1);
	cur->setIndex(-1);
	pre->setVisible(false);
	cur->setVisible(false);
	pre->offSelect();
	cur->offSelect();
	pre = NULL;
	cur = NULL;
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