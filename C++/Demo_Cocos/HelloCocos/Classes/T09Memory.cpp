#include "T09Memory.h"
#include "AppMarcos.h"

CCScene * T09Memory::scene()
{
	CCScene *scene = CCScene::create();
	T09Memory *layer = T09Memory::create();
	scene->addChild(layer);
	return scene;
}

bool T09Memory::init()
{
	CCLayer::init();
	
	array = CCArray::create();
	array->retain();
	spr = Sprite::create("HelloWorld.png");
	spr2 = CCSprite::create("hero3_tx.png");
	array->addObject(spr);
	array->addObject(spr2);
	
	/*
	CCLOG("RetainCount:%d", spr->getReferenceCount());
	spr->retain();
	CCLOG("RetainCount:%d", spr->getReferenceCount());
	spr->release();
	CCLOG("RetainCount:%d", spr->getReferenceCount());
	*/

	//schedule(schedule_selector(T09Memory::mySchedule), 2);

	CCMenuItem *item = CCMenuItemImage::create("hero1_tx.png", "hero2_tx.png", this, menu_selector(T09Memory::menuCallback));
	CCMenu *menu = CCMenu::create(item, NULL);
	this->addChild(menu);

	return true;
}

/*
void T09Memory::mySchedule(float dt)
{
	if (spr)
	{
		spr->removeFromParent();
		spr = NULL;
	}
}
*/

void T09Memory::menuCallback(CCObject *obj)
{
	//array->objectAtIndex(0);
	CCLOG("%d", array->getReferenceCount());
	CCSprite *temp = (CCSprite*)array->getObjectAtIndex(1);
	temp->setPosition(ccp(CCRANDOM_0_1() * 1024, CCRANDOM_0_1() * 768));
	addChild(temp);
	array->release();
	CCLOG("%d", array->getReferenceCount());

}