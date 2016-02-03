#include "TBack.h"
#include "TMenu.h"
#include "AppMarcos.h"

bool TBack::init()
{
	if (!Layer::init())
		return false;

	MenuItem *mainmenu = MenuItemFont::create("Main Menu", this, CC_MENU_SELECTOR(TBack::mainmenuCallBack));
	Menu *menu = Menu::create(mainmenu, NULL);
	menu->setPosition(ccp(0, 0));
	menu->setZOrder(100);
	mainmenu->setPosition(Point(winSize.width - mainmenu->boundingBox().size.width / 2, mainmenu->boundingBox().size.height / 2));
	this->addChild(menu);
	return true;
}

void TBack::mainmenuCallBack(Ref *ref)
{
	Scene *scene = TMenu::createScene();
	Director::getInstance()->replaceScene(scene);
}