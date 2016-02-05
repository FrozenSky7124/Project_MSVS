#include "T16Follow.h"
#include "AppMarcos.h"

CCScene *T16Follow::scene()
{
	CCScene * scene = CCScene::create();
	T16Follow * layer = T16Follow::create();
	scene->addChild(layer);
	return scene;
}
bool T16Follow::init()
{
	TBack::init();

	Sprite *bg = Sprite::create("map.jpg");
	bg->setAnchorPoint(ccp(0, 0));
	bg->setScale(0.7);
	addChild(bg);

	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("hero1_move_r.plist");

	hero0 = Sprite::createWithSpriteFrameName("hero1_move_r1.png");
	hero0->setPosition(ccp(50, 100));
	addChild(hero0, 0, "hero0");

	By1 = MoveBy::create(10, ccp(300, 0));
	Speed *Speed1 = Speed::create(By1, 5);
	

	CallFuncN *func = CallFuncN::create(this,
										callfuncN_selector(T16Follow::funcNCallBack));
	Sequence *seq = Sequence::create(By1, func, NULL);
	hero0->runAction(seq);

	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("hero1_move_r.png");

	Animation *animation = Animation::create();
	char filename[100];
	for (int i = 1;i <= 16;i++)
	{
		memset(filename, 0, sizeof(filename));
		sprintf(filename, "hero1_move_r%d.png", i);
		SpriteFrame *frame = SpriteFrameCache::getInstance()->spriteFrameByName(filename);
		animation->addSpriteFrame(frame);
	}
	animation->setDelayPerUnit(0.12f);
	animation->setLoops(-1);
	Animate *animate = Animate::create(animation);
	hero0->runAction(animate);

	Follow *follow = Follow::create(hero0);
	this->runAction(follow);

	return true;
}

void T16Follow::funcNCallBack(Node *node)
{
	Sprite *spr = (Sprite*)node;
	CCLOG("x = %g, y = %g", spr->getPositionX(), spr->getPositionY());
	Point worldp = this->convertToWorldSpace(spr->getPosition());
	CCLOG("x = %g, y = %g", worldp.x, worldp.y);
}