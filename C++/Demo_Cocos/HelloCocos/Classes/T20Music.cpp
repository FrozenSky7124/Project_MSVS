#include "T20Music.h"
#include "AppMarcos.h"
#include "SimpleAudioEngine.h"

CCScene *T20Music::scene()
{
	CCScene * scene = CCScene::create();
	T20Music * layer = T20Music::create();
	scene->addChild(layer);
	return scene;
}
bool T20Music::init()
{
	TBack::init();

	CocosDenshion::SimpleAudioEngine::getInstance()->preloadBackgroundMusic("audio/bg_linan.mp3");
	CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect("audio/effect_nuclear.wav");

	auto listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true);
	listener->onTouchBegan = CC_CALLBACK_2(T20Music::onTouchBegan, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic("audio/bg_linan.mp3");
	CocosDenshion::SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(0.1);

	return true;
}

bool T20Music::onTouchBegan(Touch *touch, Event *unused_event)
{
	static bool musicOnPlay = true;
	if(musicOnPlay)
	{
		CocosDenshion::SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
		musicOnPlay = false;
	}
	else
	{
		CocosDenshion::SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
		musicOnPlay = true;
	}
	return false;
}