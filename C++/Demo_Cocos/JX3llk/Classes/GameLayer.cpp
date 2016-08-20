#include "GameLayer.h"
#include "AppMacro.h"
#include "SimpleAudioEngine.h"
#include "StartMenu.h"
#include "FailLayer.h"

Scene* GameLayer::createScene()
{
	auto scene = Scene::create();

	auto layer = GameLayer::create();

	scene->addChild(layer);

	return scene;
}

bool GameLayer::init()
{
	if (!Layer::init())
	{
		return false;
	}
	
	initMap();
	initData();
	initUI();
	initSound();

	return true;
}

void GameLayer::initUI()
{
	goBackMenuItem = MenuItemImage::create("goback_normal.png", "goback_selected.png", this, SEL_MenuHandler(&GameLayer::goBackCallBack));
	MenuItem *pauseMenuItem = MenuItemImage::create("pause1.png", "pause2.png", this, SEL_MenuHandler(&GameLayer::pauseCallBack));
	goBackMenuItem->setScale(0.8f);
	pauseMenuItem->setScale(0.8f);

	//关卡
	Sprite *level = Sprite::create("level.png");
	level->setPosition(winSize.width / 16 * 1, winSize.height / 12 * 11);
	level->setScale(0.8f);
	addChild(level, 1);

	String *strlevel = String::createWithFormat("%d", _level);
	levelttf = LabelBMFont::create(strlevel->getCString(), "num.fnt");
	levelttf->setPosition(winSize.width / 16 * 1.9, winSize.height / 12 * 11 - levelttf->getBoundingBox().size.height / 4);
	levelttf->setAnchorPoint(Point(0.5, 0.5));
	levelttf->setScale(0.7f);
	addChild(levelttf, 1);

	//分数
	Sprite *score = Sprite::create("score.png");
	score->setPosition(winSize.width / 16 * 3, winSize.height / 12 * 11);
	score->setScale(0.8f);
	addChild(score, 1);

	String *strscore = String::createWithFormat("%03d", _score);
	scorettf = LabelBMFont::create(strscore->getCString(), "num.fnt");
	scorettf->setPosition(winSize.width / 16 * 4.3, winSize.height / 12 * 11 - scorettf->getBoundingBox().size.height / 4);
	scorettf->setAnchorPoint(Point(0.5, 0.5));
	scorettf->setScale(0.7f);
	addChild(scorettf, 1);

	//背景音乐
	bgmSwitchBtn = MenuItemImage::create("bgmon.png", "bgmoff.png", this, SEL_MenuHandler(&GameLayer::pauseBGM));
	bgmSwitchBtn->setScale(0.7f);
	Menu *bottomMenu = Menu::create();
	bottomMenu->setPosition(winSize.width / 16 * 14.5, winSize.height / 12 * 0.7);
	bottomMenu->addChild(bgmSwitchBtn);
	addChild(bottomMenu, 1);

	//控制菜单
	Menu *controlMenu = Menu::create();
	controlMenu->setPosition(winSize.width / 16 * 14, winSize.height / 12 * 11);
	controlMenu->addChild(pauseMenuItem);
	controlMenu->addChild(goBackMenuItem);
	controlMenu->alignItemsHorizontallyWithPadding(10.0f);
	addChild(controlMenu, 1);

	Sprite *bg = Sprite::create("gamebg1.jpg");
	bg->setAnchorPoint(Point(0.5, 0.5));
	bg->setPosition(winSize.width / 2, winSize.height / 2);
	float xScale = winSize.width / (bg->getContentSize().width);
	float yScale = winSize.height / (bg->getContentSize().height);
	bg->setScaleX(xScale);
	bg->setScaleY(yScale);
	addChild(bg, 0, 10001);

	//进度条
	Sprite *progressbgSpr = Sprite::create("timebar_bg.png");
	progressbgSpr->setPosition(Vec2(winSize.width / 16 * 9, winSize.height / 12 * 11));
	this->addChild(progressbgSpr, 1);

	_progressBar = ProgressTimer::create(Sprite::create("timebar_bar.png"));
	_progressBar->setPosition(Vec2(winSize.width / 16 * 9, winSize.height / 12 * 11));
	_progressBar->setMidpoint(ccp(0, 0));
	_progressBar->setBarChangeRate(ccp(1, 0));
	_progressBar->setType(kCCProgressTimerTypeBar);
	_progressBar->setPercentage(100.0f);
	this->addChild(_progressBar, 1);

	//进度条定时器
	schedule(SEL_SCHEDULE(&GameLayer::updateProgress), 1); //开启计时进度条定时器，设置1s触发时间

	//全局帧定时器
	scheduleUpdate();

	//剩余时间标签
	_timebar_dot = Sprite::create("timebar_dot.png");
	_timebar_dot->setScale(1.2f);
	_timebar_dot->setPosition(_progressBar->getPosition().x + _progressBar->getContentSize().width / 2, _progressBar->getPosition().y + 25);
	this->addChild(_timebar_dot, 1);

	_timeTTF = LabelTTF::create("---", "Courier New", 20);
	_timeTTF->setPosition(_progressBar->getPosition().x + _progressBar->getContentSize().width/2 , _progressBar->getPosition().y + 25);
	_timeTTF->setColor(Color3B::WHITE);
	this->addChild(_timeTTF, 1);
}

//获取关卡地图_map数据信息
void GameLayer::initData()
{
	_level = _map->getLevel();
	_score = _map->getScore();
	_remainNum = _map->_remainNum;
	bgmSwitch = true;
}

//初始化关卡1地图
void GameLayer::initMap()
{
	//MapLayer * MapLayer::create(int level, int score) 初始化_map对象，传入Level,Score数据
	_map = MapLayer::create(1, 0);
	addChild(_map, 1, 10003);
}

void GameLayer::initSound()
{
	CocosDenshion::SimpleAudioEngine::getInstance()->preloadBackgroundMusic("Sound/qixiubg1.mp3");
	CocosDenshion::SimpleAudioEngine::getInstance()->preloadBackgroundMusic("Sound/qixiubg2.mp3");
	CocosDenshion::SimpleAudioEngine::getInstance()->preloadBackgroundMusic("Sound/wanhuabg1.mp3");
	CocosDenshion::SimpleAudioEngine::getInstance()->preloadBackgroundMusic("Sound/wanhuabg2.mp3");
	CocosDenshion::SimpleAudioEngine::getInstance()->preloadBackgroundMusic("Sound/yangzhoubg.mp3");
	CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect("Sound/success.wav");
	CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect("Sound/fail.wav");
	CocosDenshion::SimpleAudioEngine::getInstance()->setEffectsVolume(0.7f);
	CocosDenshion::SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(0.3f);
	this->playSound1(0.0f);
}

//计时条定时器更新
void GameLayer::updateProgress(float delta)
{
	float currTime = _progressBar->getPercentage();
	currTime -= 1;
	_progressBar->setPercentage(currTime);

	CCString *str = CCString::createWithFormat("  %g  ", currTime);
	_timeTTF->setString(str->getCString());
	_timebar_dot->setPosition(_progressBar->getPosition().x + _progressBar->getContentSize().width / 2 - (100 - currTime) / 100 * _progressBar->getContentSize().width, _timebar_dot->getPosition().y);
	_timeTTF->setPosition(_progressBar->getPosition().x + _progressBar->getContentSize().width / 2 - (100 - currTime) / 100 * _progressBar->getContentSize().width, _timeTTF->getPosition().y);
}

//帧定时器触发
void GameLayer::update(float delta)
{
	_score = _map->getScore();
	_level = _map->getLevel();
	_remainNum = _map->_remainNum;
	float _remainTime = _progressBar->getPercentage();
	CCString *tempStr1 = String::createWithFormat("%03d", _score);
	CCString *tempStr2 = String::createWithFormat("%d", _level);
	scorettf->setString(tempStr1->getCString());
	levelttf->setString(tempStr2->getCString());
	
	//超时
	if (_remainTime <= 0)
	{
		CCLOG("Fail!");
		SAE->stopBackgroundMusic();
		bgmSwitch = false;

		//停止帧定时器
		unscheduleUpdate();
		_map->removeFromParentAndCleanup(true);
		Scene *scene = FailLayer::createScene();
		auto transition = TransitionTurnOffTiles::create(0.8f, scene);
		SAE->playEffect("Sound/fail.wav");
		Director::getInstance()->replaceScene(transition);	
	}

	//全部消除
	if (_remainNum == 0)
	{
		CCLOG("Success!");
		//重新布局
		int templevel = ++_level;
		SAE->playEffect("Sound/success.wav");
		_map->removeFromParentAndCleanup(true);
		_map = MapLayer::create(templevel, _score); //构建下一关卡地图
		this->addChild(_map);
		//重置计时器
		_progressBar->setPercentage(100);
	}
}

void GameLayer::playSound1(float dt)
{
	if (bgmSwitch == true)
	{
		SAE->stopBackgroundMusic();
		SAE->playBackgroundMusic("Sound/qixiubg1.mp3", false);
	}
	this->scheduleOnce(schedule_selector(GameLayer::playSound2), 140.0f);

	//GameBackground Change
	Sprite *bg = (Sprite*)this->getChildByTag(10001);
	bg->setTexture("gamebg1.jpg");
	float xScale = winSize.width / (bg->getContentSize().width);
	float yScale = winSize.height / (bg->getContentSize().height);
	bg->setScaleX(xScale);
	bg->setScaleY(yScale);
}

void GameLayer::playSound2(float dt)
{
	CCLOG("into playSound2");
	if (bgmSwitch == true)
	{
		SAE->stopBackgroundMusic();
		SAE->playBackgroundMusic("Sound/qixiubg2.mp3", false);
	}
	this->scheduleOnce(schedule_selector(GameLayer::playSound3), 100.0f);

	//GameBackground Change
	Sprite *bg = (Sprite*)this->getChildByTag(10001);
	bg->setTexture("gamebg2.jpg");
	float xScale = winSize.width / (bg->getContentSize().width);
	float yScale = winSize.height / (bg->getContentSize().height);
	bg->setScaleX(xScale);
	bg->setScaleY(yScale);
}

void GameLayer::playSound3(float dt)
{
	if (bgmSwitch == true)
	{
		SAE->stopBackgroundMusic();
		SAE->playBackgroundMusic("Sound/wanhuabg1.mp3", false);
	}
	this->scheduleOnce(schedule_selector(GameLayer::playSound4), 135.0f);

	//GameBackground Change
	Sprite *bg = (Sprite*)this->getChildByTag(10001);
	bg->setTexture("gamebg3.jpg");
	float xScale = winSize.width / (bg->getContentSize().width);
	float yScale = winSize.height / (bg->getContentSize().height);
	bg->setScaleX(xScale);
	bg->setScaleY(yScale);
}

void GameLayer::playSound4(float dt)
{
	if (bgmSwitch == true)
	{
		SAE->stopBackgroundMusic();
		SAE->playBackgroundMusic("Sound/wanhuabg2.mp3", false);
	}
	this->scheduleOnce(schedule_selector(GameLayer::playSound5), 105.0f);

	//GameBackground Change
	Sprite *bg = (Sprite*)this->getChildByTag(10001);
	bg->setTexture("gamebg4.jpg");
	float xScale = winSize.width / (bg->getContentSize().width);
	float yScale = winSize.height / (bg->getContentSize().height);
	bg->setScaleX(xScale);
	bg->setScaleY(yScale);
}

void GameLayer::playSound5(float dt)
{
	if (bgmSwitch == true)
	{
		SAE->stopBackgroundMusic();
		SAE->playBackgroundMusic("Sound/yangzhoubg.mp3", false);
	}
	this->scheduleOnce(schedule_selector(GameLayer::playSound1), 95.0f);

	//GameBackground Change
	Sprite *bg = (Sprite*)this->getChildByTag(10001);
	bg->setTexture("gamebg5.jpg");
	float xScale = winSize.width / (bg->getContentSize().width);
	float yScale = winSize.height / (bg->getContentSize().height);
	bg->setScaleX(xScale);
	bg->setScaleY(yScale);
}

void GameLayer::goBackCallBack(Ref *ref)
{
	//停止帧定时器
	unscheduleUpdate();
	_map->removeFromParentAndCleanup(true);

	Scene *scene = StartMenu::createScene();
	auto transition = TransitionFadeBL::create(0.8f, scene);
	SAE->stopBackgroundMusic();
	bgmSwitch = false;
	Director::getInstance()->replaceScene(transition);
}

void GameLayer::pauseCallBack(Ref *ref)
{
	static bool isPause = false;
	if (isPause == false)
	{
		//隐藏主界面
		//((MapLayer*)this->getChildByTag(10003))->setVisible(false);
		_map->setVisible(false);
		//隐藏返回键
		goBackMenuItem->setVisible(false);
		SAE->pauseBackgroundMusic();
		bgmSwitch = false;
		Director::getInstance()->pause();
		isPause = !isPause;
	}
	else
	{
		//隐藏主界面
		//((MapLayer*)this->getChildByTag(10003))->setVisible(true);
		_map->setVisible(true);
		//隐藏返回键
		goBackMenuItem->setVisible(true);
		Director::getInstance()->resume();
		SAE->resumeBackgroundMusic();
		bgmSwitch = true;
		isPause = !isPause;
	}
}

//2016-08-20
void GameLayer::pauseBGM(Ref *ref)
{
	if (bgmSwitch == true)
	{
		CCLOG("pauseBGM.");
		bgmSwitchBtn->selected();
		bgmSwitch = false;
		SAE->pauseBackgroundMusic();
	}
	else
	{
		CCLOG("resumeBGM.");
		bgmSwitchBtn->unselected();
		bgmSwitch = true;
		SAE->resumeBackgroundMusic();
	}
}