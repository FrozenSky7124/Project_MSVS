#include "T14Animation.h"
#include "AppMarcos.h"

CCScene *T14Animation::scene()
{
	CCScene * scene = CCScene::create();
	T14Animation * layer = T14Animation::create();
	scene->addChild(layer);
	return scene;
}
bool T14Animation::init()
{
	TBack::init();

	return true;
}

void T14Animation::onEnter()
{
	TBack::onEnter();

	
}

void T14Animation::onEnterTransitionDidFinish()
{
	TBack::onEnterTransitionDidFinish();
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("hero1_move_r.plist");

	Sprite *background = Sprite::create("scenebg.jpg");
	background->setPosition(ccp(winSize.width / 2, winSize.height / 2));
	addChild(background, -255, "background");

	Sprite *hero0 = Sprite::createWithSpriteFrameName("hero1_move_r1.png");
	hero0->setPosition(ccp(winSize.width / 2 - 100, winSize.height / 2));
	addChild(hero0, 0, "hero0");

	Animation *hero0_ation = Animation::create();
	char filename[100];
	for (int i = 1;i <= 16; i++)
	{
		memset(filename, 0, sizeof(filename));
		sprintf(filename, "hero1_move_r%d.png", i);
		SpriteFrame *frame = SpriteFrameCache::getInstance()->spriteFrameByName(filename);
		hero0_ation->addSpriteFrame(frame);
	}

	hero0_ation->setDelayPerUnit(0.12f);
	hero0_ation->setLoops(-1);

	Animate *animate = Animate::create(hero0_ation);

	MoveBy *by = MoveBy::create(3, Point(200, 0));
	Spawn *spawn = Spawn::create(by,
								 animate,
								 NULL);

	hero0->runAction(spawn);
}