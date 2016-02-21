#include "FailLayer.h"
#include "AppMacro.h"
#include "GameLayer.h"

Scene* FailLayer::createScene()
{
	auto scene = Scene::create();

	auto layer = FailLayer::create();

	scene->addChild(layer);

	return scene;
}

bool FailLayer::init()
{
	if (!Layer::init())
	{
		return false;
	}

	initUI();

	return true;
}

void FailLayer::initUI()
{
	//Background Image
	Sprite *bg = Sprite::create("failbg.jpg");
	bg->setAnchorPoint(Point(0.5, 0.5));
	bg->setPosition(winSize.width / 2, winSize.height / 2);
	float xScale = winSize.width / (bg->getContentSize().width);
	float yScale = winSize.height / (bg->getContentSize().height);
	bg->setScaleX(xScale);
	bg->setScaleY(yScale);
	addChild(bg, 0);

	//Menu
	MenuItem *quitGameItem = MenuItemImage::create("quitGame1.png", "quitGame2.png", this, SEL_MenuHandler(&FailLayer::quitGameCallBack));

	Menu *mainMenu = Menu::create();
	mainMenu->addChild(quitGameItem);

	mainMenu->setPosition(Point(winSize.width / 2, winSize.height / 4));
	mainMenu->alignItemsHorizontallyWithPadding(40.0f);
	addChild(mainMenu, 1);
}

void FailLayer::quitGameCallBack(Ref *ref)
{
	CCLOG("quitGameCallBack");
	Director::getInstance()->end();
}