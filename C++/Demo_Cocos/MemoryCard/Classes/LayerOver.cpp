#include "LayerOver.h"
#include "LayerStart.h"

USING_NS_CC;

Scene* LayerOver::createScene(bool status)
{
	Scene* scene = Scene::create();
	LayerOver* layer = LayerOver::create(status);
	scene->addChild(layer);
	return scene;
}
LayerOver* LayerOver::create(bool status)
{
	LayerOver* pRet = new LayerOver();
	if(pRet && pRet->init(status))
	{
		pRet->autorelease();
	}
	else
	{
		delete pRet;
		pRet = NULL;
	}
	return pRet;
}
bool LayerOver::init(bool status)
{
	if (!Layer::init())
		return false;

	Menu *menu = Menu::create();
	MenuItem *backItem = MenuItemFont::create("Back", CC_CALLBACK_1(LayerOver::menuCallBack, this));
	if (status == true)
	{
		MenuItem* statusItem = MenuItemFont::create("You Win !");
		menu->addChild(statusItem);
		menu->addChild(backItem);
	}
	else
	{
		MenuItem *statusItem = MenuItemFont::create("You Failed!");
		menu->addChild(statusItem);
		menu->addChild(backItem);
	}
	addChild(menu);
	menu->alignItemsVerticallyWithPadding(30);
		
	return true;
}

void LayerOver::menuCallBack(Object *obj)
{
	Scene *scene = LayerStart::createScene();
	CCDirector::sharedDirector()->replaceScene(scene);
}