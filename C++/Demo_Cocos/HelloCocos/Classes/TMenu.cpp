#include "TMenu.h"
#include "AppMarcos.h"
#include "T01LayerAnchorPoint.h"
#include "T02LayerSprite.h"
#include "T03LayerZorder.h"
#include "T04LayerColor.h"
#include "T05Coordinate.h"
#include "T06BatchNode.h"
#include "T07Schedule.h"
#include "T08Touch.h"
#include "T09Memory.h"
#include "T10Label.h"
#include "T11CCMenu.h"
#include "T12UI.h"
#include "T13Action.h"
#include "T14Animation.h"
#include "T15Speed.h"
#include "T16Follow.h"
#include "T17ProgressTo.h"
#include "T18Grid3D.h"
#include "T19Draw.h"
#include "T20Music.h"
#include "T21Particle.h"

static char* menu_array[] =
{
	"T01LayerAnchorPoint.h",
	"T02LayerSprite.h",
	"T03LayerZorder.h",
	"T04LayerColor.h",
	"T05Coordinate.h",
	"T06BatchNode.h",
	"T07Schedule.h",
	"T08Touch.h",
	"T09Memory.h",
	"T10Label.h",
	"T11CCMenu.h",
	"T12UI.h",
	"T13Action.h",
	"T14Animation.h",
	"T15Speed.h",
	"T16Follow.h",
	"T17ProgressTo.h",
	"T18Grid3D.h",
	"T19Draw.h",
	"T20Music.h",
	"T21Particle.h"
};

Scene* TMenu::createScene()
{
	Scene* scene = Scene::create();
	TMenu* layer = TMenu::create();
	scene->addChild(layer);
	return scene;
}

bool TMenu::init()
{
	if(!Layer::init())
		return false;

	Menu* menu = Menu::create();
	for (int i = 0;i < sizeof(menu_array) / sizeof(*menu_array);i++)
	{
		MenuItem *item = MenuItemFont::create(menu_array[i], this, CC_MENU_SELECTOR(TMenu::menuCallBack));
		item->setTag(i);
		menu->addChild(item);
	}
	menu->alignItemsVertically();
	menu->setTag(10001);
	this->addChild(menu);

	MenuItem *closeItem = MenuItemImage::create("CloseNormal.png", "CloseSelected.png",
												this,
												CC_MENU_SELECTOR(TMenu::closeCallBack));
	closeItem->setPosition(Point(winSize.width / 2 - closeItem->getContentSize().width / 2, 0 - (winSize.height / 2 - closeItem->getContentSize().height / 2)));
	Menu* menu2 = Menu::create(closeItem, NULL);
	this->addChild(menu2);

	//Touch
	auto *listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true);
	listener->onTouchBegan = CC_CALLBACK_2(TMenu::onTouchBegan, this);
	listener->onTouchMoved = CC_CALLBACK_2(TMenu::onTouchMoved, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	return true;
}

void TMenu::menuCallBack(Ref *ref)
{
	int idx = ((MenuItem*)ref)->getTag() + 1;
	CCLOG("%d", idx);
	Scene* scene = NULL;
	switch (idx)
	{
	case 1:
		scene = T01LayerAnchorPoint::scene();
		break;
	case 2:
		scene = T02LayerSprite::scene();
		break;
	case 3:
		scene = T03LayerZorder::scene();
		break;
	case 4:
		scene = T04LayerColor::scene();
		break;
	case 5:
		scene = T05Coordinate::scene();
		break;
	case 6:
		scene = T06BatchNode::scene();
		break;
	case 7:
		scene = T07Schedule::scene();
		break;
	case 8:
		scene = T08Touch::scene();
		break;
	case 9:
		scene = T09Memory::scene();
		break;
	case 10:
		scene = T10Label::scene();
		break;
	case 11:
		scene = T11CCMenu::scene();
		break;
	case 12:
		scene = T12UI::scene();
		break;
	case 13:
		scene = T13Action::scene();
		break;
	case 14:
		scene = T14Animation::scene();
		break;
	case 15:
		scene = T15Speed::scene();
		break;
	case 16:
		scene = T16Follow::scene();
		break;
	case 17:
		scene = T17ProgressTo::scene();
		break;
	case 18:
		scene = T18Grid3D::scene();
		break;
	case 19:
		scene = T19Draw::scene();
		break;
	case 20:
		scene = T20Music::scene();
		break;
	case 21:
		scene = T21Particle::scene();
		break;
	default:
		break;
	}
	if(scene!=NULL)
	{
		TransitionScene *pScene = TransitionTurnOffTiles::create(2, scene);
		Director::getInstance()->replaceScene(pScene);

	}
}

void TMenu::closeCallBack(Ref* ref)
{
	Director::getInstance()->end();
}

bool TMenu::onTouchBegan(Touch *touch, Event *event)
{
	return true;
}

void TMenu::onTouchMoved(Touch *touch, Event *event)
{
	auto menu = TMenu::getChildByTag(10001);
	int num = menu->getChildrenCount() - 1;
	
	float offsetY = (0 - (((MenuItem*)(menu->getChildByTag(num))))->getPositionY()) - winSize.height / 2 + 20;
	float offsetC = menu->getPositionY() - winSize.height / 2;

	if ((0 - offsetC) > offsetY && (offsetC < 0) && (touch->getDelta().y < 0))
	{
		//CCLOG("MenuContentSize: x %g,y %g", menu->getContentSize().width, menu->getContentSize().height);
		//CCLOG("offsetC = %g", offsetC);
		//CCLOG("offsetY = %g", offsetY);
		return;
	}
	if ((offsetC) > offsetY && (offsetC > 0) && (touch->getDelta().y > 0))
	{
		//CCLOG("MenuContentSize: x %g,y %g", menu->getContentSize().width, menu->getContentSize().height);
		//CCLOG("offsetC = %g", offsetC);
		//CCLOG("offsetY = %g", offsetY);
		return;
	}
	menu->setPosition(Point(menu->getPositionX(), menu->getPositionY() + touch->getDelta().y));

}