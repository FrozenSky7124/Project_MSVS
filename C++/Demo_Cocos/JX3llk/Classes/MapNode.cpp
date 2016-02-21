#include "MapNode.h"
#include "AppMacro.h"

bool MapNode::init(int index)
{
	if (!Sprite::init())
	{
		return false;
	}

	this->_index = index;

	String *strIndex = String::createWithFormat("NodeImg%d.png", index);
	this->setTexture(strIndex->getCString());

	bg = Sprite::create("NodeImgBg.png");
	bg->setAnchorPoint(Vec2(0, 0));
	bg->setVisible(false);
	addChild(bg, 5);

	return true;
}

MapNode * MapNode::create(int index)
{
	MapNode *pRet = new MapNode;
	if (pRet && pRet->init(index))
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

void MapNode::setX(int x)
{
	this->_x = x;
}

int MapNode::getX()
{
	return this->_x;
}

void MapNode::setY(int y)
{
	this->_y = y;
}

int MapNode::getY()
{
	return this->_y;
}

void MapNode::setIndex(int index)
{
	this->_index = index;
}

int MapNode::getIndex()
{
	return this->_index;
}

void MapNode::onSelect()
{
	bg->setVisible(true);
}

void MapNode::offSelect()
{
	bg->setVisible(false);
}