#include "T10Label.h"
#include "AppMarcos.h"

CCScene * T10Label::scene()
{
	CCScene *scene = CCScene::create();
	T10Label *layer = T10Label::create();
	scene->addChild(layer);
	return scene;
}

bool T10Label::init()
{
	CCLayer::init();

	//CCLabelTTF

	CCLabelTTF *lttf = CCLabelTTF::create("仙剑奇侠传", "华文行楷", 50);
	lttf->setPosition(ccp(winSize.width / 2, winSize.height / 2));
	this->addChild(lttf);
	lttf->setFontName("Courier New");
	lttf->setString("xxxx");
	lttf->setTag(LTTF);

	//CCLabelAtlas

	//CCLabelAtlas *latlas = CCLabelAtlas::create("67315", "num1.png", 32, 33, '0');
	//latlas->setAnchorPoint(ccp(0.5, 0.5));
	//latlas->setPosition(ccp(winSize.width / 2, winSize.height / 2 + 100));
	//this->addChild(latlas);

	//CCLabelBMFont


	CCLabelBMFont *lbm = CCLabelBMFont::create("+71245690", "num1.fnt");
	lbm->setAnchorPoint(ccp(0.5, 0.5));
	lbm->setScale(2.5);
	lbm->setOpacity(100);
	lbm->setPosition(ccp(winSize.width / 2, winSize.height / 2 + 150));
	lbm->setTag(LBM);
	this->addChild(lbm);

	CCLabelBMFont *lbm1 = CCLabelBMFont::create("+71245690", "num1.fnt");
	lbm1->setScale(2.5);
	lbm1->setAnchorPoint(ccp(0.5, 0.5));
	lbm1->setPosition(ccp(winSize.width / 2, winSize.height / 2 + 150));
	this->addChild(lbm1);

	//获取CCLabelBMFont的每个子节点
	
	for (int i = 0;i < 9; i++)
	{
		CCSprite * spr = (CCSprite*)lbm1->getLetter(i);
		spr->setAnchorPoint(ccp(0.5, 0.5));
		spr->setPosition(lbm1->convertToNodeSpace(ccp(CCRANDOM_0_1() * 1024, CCRANDOM_0_1() * 768)));
		//spr->setPosition(lbm1->convertToNodeSpace(ccp(winSize.width / 2, winSize.height / 2 + 150)));
	}

	for (int i = 0;i < 9; i++)
	{
		static float x = winSize.width / 2 - 130;
		static float y = winSize.height / 2 + 150;
		CCSprite * spr = (CCSprite*)lbm1->getLetter(i);
		CCMoveTo * move = CCMoveTo::create(2, lbm1->convertToNodeSpace(ccp(x += 50, y)));
		//CCMoveTo * move = CCMoveTo::create(2, lbm1->convertToNodeSpaceAR(ccp(x += 50, y)));
		//CCMoveTo * move = CCMoveTo::create(2, ccp(x += 50, y));
		spr->runAction(move);	
	}



	/*
	CCArray *fontarray = CCArray::createWithObject(lbm1);
	CCObject *obj;
	CCARRAY_FOREACH(fontarray,obj)
	{
		CCSprite * spr = (CCSprite*)obj;
		spr->setPosition(ccp(CCRANDOM_0_1() * 1000, CCRANDOM_0_1() * 760));
	}
	*/

	schedule(schedule_selector(T10Label::myTimeCount1), 1);
	//schedule(schedule_selector(T10Label::myTimeCount2), 1);
	return true;
}

void T10Label::myTimeCount1(float dt)
{
	static float count1 = 0;
	count1 += dt;
	CCString *scount = CCString::createWithFormat("%d", int(count1));
	CCLabelTTF *lttf = (CCLabelTTF*)getChildByTag(LTTF);
	lttf->setString(scount->getCString());
}

void T10Label::myTimeCount2(float dt)
{
	static float count2 = 0;
	count2 += dt;
	CCString *scount = CCString::createWithFormat("%d", int(count2));
	CCLabelTTF *lbm = (CCLabelTTF*)getChildByTag(LBM);
	lbm->setString(scount->getCString());
}