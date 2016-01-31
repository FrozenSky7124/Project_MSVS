#include "LayerStart.h"
#include "LayerGame.h"

USING_NS_CC;

Scene* LayerStart::createScene()
{
	auto scene = Scene::create();

	auto layer = LayerStart::create();

	scene->addChild(layer);

	return scene;
}

bool LayerStart::init()
{
	if (!Layer::init())
	{
		return false;
	}

	CCMenuItem *easyItem = CCMenuItemFont::create("Easy", this, menu_selector(LayerStart::easy));
	CCMenuItem *hardItem = CCMenuItemFont::create("Hard", this, menu_selector(LayerStart::hard));
	CCMenuItem *veryItem = CCMenuItemFont::create("VeryHard", this, menu_selector(LayerStart::very));
	CCMenuItem *quitItem = CCMenuItemFont::create("Quit", this, menu_selector(LayerStart::quit));

	CCMenu *menu = CCMenu::create(easyItem, hardItem, veryItem, quitItem, NULL);
	menu->alignItemsVerticallyWithPadding(30.0);
	addChild(menu);
	return true;
}

void LayerStart::easy(CCObject *obj)
{
	Scene *scene = LayerGame::createScene(EASY);
	CCDirector::sharedDirector()->replaceScene(scene);
}
void LayerStart::hard(CCObject *obj)
{
	Scene *scene = LayerGame::createScene(HARD);
	CCDirector::sharedDirector()->replaceScene(scene);
}
void LayerStart::very(CCObject *obj)
{
	Scene *scene = LayerGame::createScene(VERYHARD);
	CCDirector::sharedDirector()->replaceScene(scene);
}
void LayerStart::quit(CCObject *obj)
{
	CCDirector::getInstance()->end();
}