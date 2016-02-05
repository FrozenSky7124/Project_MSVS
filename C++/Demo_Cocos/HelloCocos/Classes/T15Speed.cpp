#include "T15Speed.h"
#include "AppMarcos.h"

CCScene *T15Speed::scene()
{
	CCScene * scene = CCScene::create();
	T15Speed * layer = T15Speed::create();
	scene->addChild(layer);
	return scene;
}
bool T15Speed::init()
{
	TBack::init();

	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("hero1_move_r.plist");

	hero0 = Sprite::createWithSpriteFrameName("hero1_move_r1.png");
	hero0->setPosition(ccp(100, 160));
	addChild(hero0, 0, "hero0");

	power = Sprite::create("CloseSelected.png");
	power->setPosition(ccp(200, 160));
	addChild(power);

	By1 = MoveBy::create(10, ccp(300, 0));
	Speed *Speed1 = Speed::create(By1, 5);
	hero0->runAction(By1);

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

	scheduleUpdate();

	return true;
}

void T15Speed::update(float dt)
{
	//Åö×²¼ì²â±ß¾à¿ÙÍ¼
	Rect heroRect = Rect(hero0->boundingBox().origin.x + 20,
						 hero0->boundingBox().origin.y,
						 hero0->boundingBox().size.width - 40,
						 hero0->boundingBox().size.height);

	if ((power != NULL) && (heroRect.intersectsRect(power->boundingBox())))
	{
		power->removeFromParentAndCleanup(true);
		power = NULL;

		Speed *speed = Speed::create(By1, 5);
		hero0->runAction(speed);
	}
}