#include "StartMenu.h"
#include "AppMacro.h"
#include "GameLayer.h"

Scene* StartMenu::createScene()
{
	auto scene = Scene::create();

	auto layer = StartMenu::create();

	scene->addChild(layer);

	return scene;
}

bool StartMenu::init()
{
	if (!Layer::init())
	{
		return false;
	}

	initUI();
	debug();

	return true;
}

void StartMenu::initUI()
{
	Sprite *bg = Sprite::create("startbg.jpg");
	bg->setAnchorPoint(Point(0.5, 0.5));
	bg->setPosition(winSize.width / 2, winSize.height / 2);
	float xScale = winSize.width / (bg->getContentSize().width);
	float yScale = winSize.height / (bg->getContentSize().height);
	bg->setScaleX(xScale);
	bg->setScaleY(yScale);
	addChild(bg, 0, 10001);

	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("MenuSprite.plist");

	Sprite *logo = Sprite::create("logo.png");
	logo->setPosition(winSize.width / 2, winSize.height / 4 * 3);
	logo->setScale(0.8);
	addChild(logo, 1);

	Sprite *cj = Sprite::createWithSpriteFrameName("cj.png");
	cj->setScale(0.5);
	cj->setPosition(winSize.width / 5 * 1, cj->getBoundingBox().size.height / 2);
	addChild(cj);

	Sprite *xx = Sprite::createWithSpriteFrameName("xx.png");
	xx->setScale(0.5);
	xx->setPosition(winSize.width / 5 * 2, xx->getBoundingBox().size.height / 2);
	addChild(xx);

	Sprite *sl = Sprite::createWithSpriteFrameName("sl.png");
	sl->setScale(0.5);
	sl->setPosition(winSize.width / 5 * 3, sl->getBoundingBox().size.height / 2);
	addChild(sl);

	Sprite *wh = Sprite::createWithSpriteFrameName("wh.png");
	wh->setScale(0.5);
	wh->setPosition(winSize.width / 5 * 4, wh->getBoundingBox().size.height / 2);
	addChild(wh);

	Sprite *cy = Sprite::createWithSpriteFrameName("cy.png");
	cy->setScale(0.5);
	cy->setPosition(winSize.width / 5 * 5, cy->getBoundingBox().size.height / 2);
	addChild(cy);

	Sprite *tc = Sprite::createWithSpriteFrameName("tc.png");
	tc->setScale(0.5);
	tc->setPosition(winSize.width / 5 * 6, tc->getBoundingBox().size.height / 2);
	addChild(tc);

	Sprite *wd = Sprite::createWithSpriteFrameName("wd.png");
	wd->setScale(0.5);
	wd->setPosition(winSize.width / 5 * 7, wd->getBoundingBox().size.height / 2);
	addChild(wd);

	//Animation
	MoveBy *by1 = MoveBy::create(20, Point(0 - winSize.width*2, 0));
	MoveBy *by2 = MoveBy::create(20, Point(0 - winSize.width*2, 0));
	MoveBy *by3 = MoveBy::create(20, Point(0 - winSize.width*2, 0));
	MoveBy *by4 = MoveBy::create(20, Point(0 - winSize.width*2, 0));
	MoveBy *by5 = MoveBy::create(20, Point(0 - winSize.width*2, 0));
	MoveBy *by6 = MoveBy::create(20, Point(0 - winSize.width*2, 0));
	MoveBy *by7 = MoveBy::create(20, Point(0 - winSize.width*2, 0));

	cj->runAction(RepeatForever::create(by1));
	xx->runAction(RepeatForever::create(by2));
	sl->runAction(RepeatForever::create(by3));
	wh->runAction(RepeatForever::create(by4));
	cy->runAction(RepeatForever::create(by5));
	tc->runAction(RepeatForever::create(by6));
	wd->runAction(RepeatForever::create(by7));

	//Operate
	spriteVector = new Vector<Sprite*>;
	spriteVector->pushBack(cj);
	spriteVector->pushBack(xx);
	spriteVector->pushBack(sl);
	spriteVector->pushBack(wh);
	spriteVector->pushBack(cy);
	spriteVector->pushBack(tc);
	spriteVector->pushBack(wd);

	scheduleUpdate();

	//Menu
	MenuItem *startGameItem = MenuItemImage::create("startGame1.png", "startGame2.png", this, SEL_MenuHandler(&StartMenu::startGameCallBack));
	MenuItem *quitGameItem = MenuItemImage::create("quitGame1.png", "quitGame2.png", this, SEL_MenuHandler(&StartMenu::quitGameCallBack));
	MenuItem *rankGameItem = MenuItemImage::create("rankGame1.png", "rankGame2.png", this, SEL_MenuHandler(&StartMenu::rankGameCallBack));

	Menu *mainMenu = Menu::create();
	mainMenu->addChild(startGameItem);
	mainMenu->addChild(rankGameItem);
	mainMenu->addChild(quitGameItem);

	mainMenu->setPosition(Point(winSize.width / 2, winSize.height / 2));
	mainMenu->alignItemsHorizontallyWithPadding(40.0f);
	addChild(mainMenu, 2);
}

void StartMenu::update(float delta)
{
	for (int i = 0;i < spriteVector->size();i++)
	{
		Sprite *spr = (Sprite*)spriteVector->at(i);
		if (spr->getPosition().x <= (0 - winSize.width/5))
		{
			spr->setPosition(winSize.width / 5 * 6, spr->getBoundingBox().size.height / 2);
		}
	}
}

void StartMenu::startGameCallBack(Ref *ref)
{
	CCLOG("startGameCallBack");
	Scene *scene = GameLayer::createScene();
	auto transition = TransitionFadeTR::create(1.0f, scene);
	Director::getInstance()->replaceScene(transition);
}

void StartMenu::quitGameCallBack(Ref *ref)
{
	CCLOG("quitGameCallBack");
	Director::getInstance()->end();
}

void StartMenu::rankGameCallBack(Ref *ref)
{
	CCLOG("rankGameCallBack");
}

void StartMenu::debug()
{
	CCLOG("Visible Size: %g %g", Director::getInstance()->getVisibleSize().width, Director::getInstance()->getVisibleSize().height);
	CCLOG("Win Size: %g %g", Director::getInstance()->getWinSize().width, Director::getInstance()->getWinSize().height);

	Sprite *spr = (Sprite*)(getChildByTag(10001));
	CCLOG("bg content %g %g", spr->getContentSize().width, spr->getContentSize().height);
	CCLOG("bg bounding %g %g", spr->getBoundingBox().size.width, spr->getBoundingBox().size.height);

	auto director = Director::getInstance();
	auto glview = director->getOpenGLView();
	CCLOG("gl frame size %g %g", glview->getFrameSize().width, glview->getFrameSize().height);

	CCLOG("spriteVector.Size = %d", spriteVector->size());
}
