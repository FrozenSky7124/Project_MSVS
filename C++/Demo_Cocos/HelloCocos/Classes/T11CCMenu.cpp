#include "T11CCMenu.h"
#include "AppMarcos.h"

CCScene * T11CCMenu::scene()
{
	CCScene *scene = CCScene::create();
	T11CCMenu *layer = T11CCMenu::create();
	scene->addChild(layer);
	return scene;
}

bool T11CCMenu::init()
{
	TBack::init();

	CCMenuItem *menuItem1 = CCMenuItemFont::create("menuItem1");
	menuItem1->setTarget(this, CC_MENU_SELECTOR(T11CCMenu::menuItem1_callback));
	menuItem1->setTag(1);

	CCMenuItem *menuItem2 = CCMenuItemFont::create("menuItem2");
	menuItem2->setTarget(this, CC_MENU_SELECTOR(T11CCMenu::menuItem1_callback));
	menuItem2->setTag(2);

	CCMenuItem *menuImg1 = CCMenuItemImage::create("button_normal.png", "button_click.png", this,
												   CC_MENU_SELECTOR(T11CCMenu::menuItem1_callback));
	menuImg1->setTag(3);

	LabelBMFont *bm = LabelBMFont::create("1234", "num1.fnt", 0.0, TextHAlignment::LEFT, Point(0, 0));
	CCMenuItem *menuBm = CCMenuItemLabel::create(bm, this, CC_MENU_SELECTOR(T11CCMenu::menuItem1_callback));
	menuBm->setTag(4);

	CCMenuItem *t1 = CCMenuItemFont::create("2x FXAA");
	CCMenuItem *t2 = CCMenuItemFont::create("4x FXAA");
	CCMenuItem *t3 = CCMenuItemFont::create("8x FXAA");
	CCMenuItem *t4 = CCMenuItemFont::create("16x FXAA");
	CCMenuItem *menuToggle = CCMenuItemToggle::createWithTarget(this, CC_MENU_SELECTOR(T11CCMenu::menuItem1_callback),
																t1, t2, t3, t4, NULL);

	CCMenu *menu = CCMenu::create();
	menu->addChild(menuItem1);
	menu->addChild(menuItem2);
	menu->addChild(menuImg1);
	menu->addChild(menuBm);
	menu->addChild(menuToggle);
	menu->alignItemsVerticallyWithPadding(10);
	menuImg1->setPosition(Point(winSize.width / 2 - menuImg1->getContentSize().width / 2, 0 - winSize.height / 2 + menuImg1->getContentSize().height / 2));
	menuBm->setPosition(Point(0 - (winSize.width / 2 - menuImg1->getContentSize().width / 2), 0 - winSize.height / 2 + menuImg1->getContentSize().height / 2));

	
	this->addChild(menu);

	CCLOG("menuItem1 Position.x = %g, Position.y = %g", menuItem1->getPositionX(), menuItem1->getPositionY());
	CCLOG("menuItem1 Anchor.x = %g, Anchor.y = %g", menuItem1->getAnchorPoint().x, menuItem1->getAnchorPoint().y);

	CCLOG("menu Position.x = %g, Position.y = %g", menu->getPositionX(), menu->getPositionY());
	CCLOG("menu Anchor.x = %g, Anchor.y = %g", menu->getAnchorPoint().x, menu->getAnchorPoint().y);
	return true;
}

void T11CCMenu::menuItem1_callback(Ref *ref)
{
	CCLOG("%d", ((CCMenuItem*)ref)->getTag());
}