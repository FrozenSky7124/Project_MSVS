#include "CardItem.h"

USING_NS_CC;

CardItem * CardItem::create(int idx)
{
	CardItem *pRet = new CardItem;
	if(pRet && pRet->init(idx))
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

bool CardItem::init(int idx)
{
	if(!CCSprite::init())
		return false;

	setContentSize(CCSizeMake(80, 80));
	setAnchorPoint(ccp(0, 0));

	CCString *idxstr = CCString::createWithFormat("%d", idx);


	ttf = CCLabelTTF::create(idxstr->getCString(), "Courier New", 50);
	ttf->setPosition(ccp(40, 40));
	this->addChild(ttf);


	bg = CCSprite::create();
	bg->setTextureRect(CCRectMake(0, 0, 77, 77));
	bg->setAnchorPoint(ccp(0, 0));
	bg->setPosition(ccp(0, 0));
	this->addChild(bg);

	return true;
}

void CardItem::showWhite()
{
	ttf->setVisible(false);
	bg->setVisible(true);
}

void CardItem::showText()
{
	ttf->setVisible(true);
	ttf->setColor(ccc3(0, 0, 0));
	ttf->setZOrder(1);
	bg->setVisible(true);
	bg->setColor(ccc3(102, 204, 255));
	bg->setZOrder(0);
}