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
	this->z1 = NULL;
	this->z2 = NULL;
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

	//�����ؿ���ͼ����Χ��id = -1
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
		//����MapLayer��Χ
		return true;
	}

	return false;
}

void MapLayer::onTouchEnded(Touch *touch, Event *event)
{
	if (_preNode == NULL)
	{
		onMapNode(&_preNode, touch);
		if (_preNode->getIndex() != -1)
		{
			_preNode->onSelect();
			return;
		}
		_preNode = NULL;
		return;
	}
	else if (_preNode == onMapNode(&_curNode, touch))
	{
		//_preNode��_curNode��ͬ��ѡ����ͬһ����
		if (_curNode->getIndex() != -1)
		{
			_preNode->offSelect();
			CCLOG("_preNode == _curNode");
			_preNode = NULL;
			_curNode = NULL;
			return;
		}
		_curNode = NULL;
		return;
	}
	else
	{
		//_preNode��_curNode��ͬ��ѡ����������
		if (_curNode->getIndex() != -1)
		{
			_curNode->onSelect();
			if (isSameIndex(_preNode, _curNode) && match(_preNode, _curNode) == true)
			{
				//��������
				clearNode(_preNode, _curNode);
				return;
			}
			else
			{
				//����������
				_preNode->offSelect();
				_preNode = _curNode;
				_curNode = NULL;
				return;
			}
		}
		_curNode = NULL;
		return;
	}
}

MapNode * MapLayer::onMapNode(MapNode** temp, Touch *touch)
{
	for (int i = 0; i < realArray->count(); i++)
	{
		MapNode *tempNode = (MapNode*)realArray->getObjectAtIndex(i);

		if (tempNode->getBoundingBox().containsPoint(this->convertToNodeSpace(touch->getLocation())))
		{
			//���д��״̬��MapNode
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

//�ж��Ƿ����ͨ
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

//ֱ����ͨ�жϺ���
bool MapLayer::matchDirect(MapNode *pre, MapNode *cur)
{
	if (!((pre->getX() == cur->getX()) || (pre->getY() == cur->getY())))
	{
		return false;
	}
	CCLOG("Into matchDirect Function!");
	//�жϺ�����ͨ
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

	//�ж�������ͨ
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
	if (match_x || match_y)
	{
		z1 = NULL;
		z2 = NULL;
	}
	return match_x || match_y;
}

//��1���۵����ͨ�жϺ���
bool MapLayer::matchOnecorner(MapNode *pre, MapNode *cur)
{
	//����۵�
	int x1 = pre->getX();
	int y1 = cur->getY();
	int x2 = cur->getX();
	int y2 = pre->getY();

	MapNode *temp1 = (MapNode *)totalArray->getObjectAtIndex(y1 * (MapColumn + 2) + x1);
	MapNode *temp2 = (MapNode *)totalArray->getObjectAtIndex(y2 * (MapColumn + 2) + x2);

	if ((temp1->getIndex() == -1) && (matchDirect(pre, temp1)) && (matchDirect(cur, temp1)))
	{
		/*��¼�۵�z1*/
		z1 = temp1;
		return true;
	}

	if ((temp2->getIndex() == -1) && (matchDirect(pre, temp2)) && (matchDirect(cur, temp2)))
	{
		/*��¼�۵�z1*/
		z1 = temp2;
		return true;
	}
	//�޷��ҵ�1���۵��·������z2 = NULL
	z1 = NULL;
	return false;
}

//��2���۵����ͨ�жϺ���
bool MapLayer::matchTwocorner(MapNode *pre, MapNode *cur)
{
	int x = pre->getX();
	int y = pre->getY();
	
	//LeftѰ·
	for (int i = x - 1; i >= 0; i--)
	{
		if (!(PointToMapNode(i, y)->getIndex() == -1))
		{
			break;
		}
		if (matchOnecorner(PointToMapNode(i, y), cur))
		{
			//��¼�۵�
			z2 = PointToMapNode(i, y);
			return true;
		}
	}
	//RightѰ·
	for (int i = x + 1; i <= MapColumn + 1; i++)
	{
		if (!(PointToMapNode(i, y)->getIndex() == -1))
		{
			break;
		}
		if (matchOnecorner(PointToMapNode(i, y), cur))
		{
			//��¼�۵�
			z2 = PointToMapNode(i, y);
			return true;
		}
	}
	//TopѰ·
	for (int i = y + 1; i <= MapRow + 1; i++)
	{
		if (!(PointToMapNode(x, i)->getIndex() == -1))
		{
			break;
		}
		if (matchOnecorner(PointToMapNode(x, i), cur))
		{
			//��¼�۵�
			z2 = PointToMapNode(x, i);
			return true;
		}
	}
	//BottomѰ·
	for (int i = y - 1; i >= 0; i--)
	{
		if (!(PointToMapNode(x, i)->getIndex() == -1))
		{
			break;
		}
		if (matchOnecorner(PointToMapNode(x, i), cur))
		{
			//��¼�۵�
			z2 = PointToMapNode(x, i);
			return true;
		}
	}
	z2 = NULL;
	return false;
}

//���ͼ�飺ͼ��Index = -1
void MapLayer::clearNode(MapNode *pre, MapNode *cur)
{
	CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("Sound/biubiubiu.wav");
	this->_score++;
	this->_remainNum = this->_remainNum - 2;
	pre->setIndex(-1);
	cur->setIndex(-1);
	//pre->setVisible(false);
	//cur->setVisible(false);
	pre->setTexture("NodeImg-1.png");
	cur->setTexture("NodeImg-1.png");
	this->linkedp = pre;
	this->linkedc = cur;
	if (z1 == NULL && z2 == NULL)
	{
		linkNode(pre, cur);
		this->scheduleOnce(schedule_selector(MapLayer::clearAllLink), 0.1f);
	}
	else if (z1 != NULL && z2 == NULL)
	{
		linkNode(pre, z1);
		linkNode(z1, cur);
		this->scheduleOnce(schedule_selector(MapLayer::clearAllLink), 0.1f);
	}
	else if (z1 != NULL && z2 != NULL)
	{
		CCLOG("z1 = %d %d z2 = %d %d\n", z1->getX(), z1->getY(), z2->getX(), z2->getY());
		linkNode(pre, z2);
		linkNode(z2, z1);
		linkNode(z1, cur);
		this->scheduleOnce(schedule_selector(MapLayer::clearAllLink), 0.1f);
	}
	//z1 = z2 = NULL;
	pre->offSelect();
	cur->offSelect();
	pre = NULL;
	cur = NULL;
}

void MapLayer::linkNode(MapNode *pre, MapNode *cur)
{
	/*�����㷨*/
	//MapNode *pre, MapNode *cur
	int px = pre->getX();
	int py = pre->getY();
	int cx = cur->getX();
	int cy = cur->getY();
	if (px == cx && py > cy)
	{
		for (int i = cy; i <= py; i++)
		{
			((Sprite *)(PointToMapNode(px, i)->onLink))->setVisible(true);
		}
	}
	else if (px == cx && py < cy)
	{
		for (int i = py; i <= cy; i++)
		{
			((Sprite *)(PointToMapNode(px, i)->onLink))->setVisible(true);
		}
	}
	if (py == cy && px > cx)
	{
		for (int i = cx; i <= px; i++)
		{
			((Sprite *)(PointToMapNode(i, py)->onLink))->setVisible(true);
		}
	}
	else if (py == cy && px < cx)
	{
		for (int i = px; i <= cx; i++)
		{
			((Sprite *)(PointToMapNode(i, py)->onLink))->setVisible(true);
		}
	}
}

void MapLayer::clearLinkNode(MapNode *pre, MapNode *cur)
{
	/*�����㷨*/
	//MapNode *pre, MapNode *cur
	int px = pre->getX();
	int py = pre->getY();
	int cx = cur->getX();
	int cy = cur->getY();
	if (px == cx && py > cy)
	{
		for (int i = cy; i <= py; i++)
		{
			((Sprite *)(PointToMapNode(px, i)->onLink))->setVisible(false);
		}
	}
	else if (px == cx && py < cy)
	{
		for (int i = py; i <= cy; i++)
		{
			((Sprite *)(PointToMapNode(px, i)->onLink))->setVisible(false);
		}
	}
	if (py == cy && px > cx)
	{
		for (int i = cx; i <= px; i++)
		{
			((Sprite *)(PointToMapNode(i, py)->onLink))->setVisible(false);
		}
	}
	else if (py == cy && px < cx)
	{
		for (int i = px; i <= cx; i++)
		{
			((Sprite *)(PointToMapNode(i, py)->onLink))->setVisible(false);
		}
	}
}

void MapLayer::clearAllLink(float dt)
{
	CCLOG("ClearAllLink");
	if (z1 == NULL && z2 == NULL)
	{
		clearLinkNode(linkedp, linkedc);
	}
	else if (z1 != NULL && z2 == NULL)
	{
		clearLinkNode(linkedp, z1);
		clearLinkNode(z1, linkedc);
	}
	else if (z1 != NULL && z2 != NULL)
	{
		clearLinkNode(linkedp, z2);
		clearLinkNode(z2, z1);
		clearLinkNode(z1, linkedc);
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