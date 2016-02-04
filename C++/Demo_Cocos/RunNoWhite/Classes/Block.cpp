#include "Block.h"
#include "AppMacros.h"

CCArray * Block::array = NULL;

Block* Block::create(Size size, Color3B color,
					 CCString str, float strSize, Color3B strColor)
{
	if (array == NULL)
	{
		//初始化Block数组
		array = CCArray::create();
		array->retain();
	}

	Block * pRet = new Block;
	if (pRet && pRet->init(size, color, str, strSize, strColor))
	{
		pRet->autorelease();
		array->addObject(pRet);
	}
	else
	{
		delete pRet;
		pRet = NULL;
	}
	return pRet;
}

bool Block::init(Size size, Color3B color,
		  CCString str, float strSize, Color3B strColor)
{
	if (!Sprite::init())
	{
		return false;
	}

	setContentSize(size);
	setTextureRect(Rect(0, 0, size.width, size.height));
	setColor(color);
	setAnchorPoint(Point(0, 0));

	LabelTTF *ttf = LabelTTF::create();
	ttf->setString(str.getCString());
	ttf->setFontSize(strSize);
	ttf->setColor(strColor);
	ttf->setPosition(Point(size.width / 2, size.height / 2));
	
	addChild(ttf);

	return true;
}

CCArray * Block::getBlockArray()
{
	return array;
}

void Block::moveDownAndCleanUp()
{
	_lineIndex--;
	MoveTo *movedown = MoveTo::create(0.01, Point(getPositionX(), getPositionY() - winSize.height / 4));
	runAction(movedown);
	if(_lineIndex<0)
	{
		array->removeObject(this);
		removeFromParentAndCleanup(true);
	}
}