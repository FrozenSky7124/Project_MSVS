#ifndef __GameLayer_H__
#define __GameLayer_H__

#include "cocos2d.h"
#include "MapLayer.h"

USING_NS_CC;

class GameLayer : public Layer
{
public:
	static Scene* createScene();

	//游戏界面主要初始化函数：将初始化MapLayer对象_map构建连连看图块布局
	virtual bool init();

	CREATE_FUNC(GameLayer);

	int _level; //存储从_map对象获取的关卡信息
	int _score; //存储从_map对象获取的分数信息
	int _remainNum; //存储从_map对象获取的当前关卡剩余图块数量

private:
	ProgressTimer * _progressBar;
	LabelTTF * _timeTTF;
	Sprite * _timebar_dot;
	MapLayer * _map;
	MenuItem *goBackMenuItem;
	LabelBMFont *scorettf;
	LabelBMFont *levelttf;
	
	void initUI();
	void initData();
	void initMap();
	void initSound();
	void playSound1(float dt);
	void playSound2(float dt);
	void playSound3(float dt);
	void playSound4(float dt);
	void playSound5(float dt);
	void updateProgress(float delta);
	void update(float delta);

	void goBackCallBack(Ref *ref);
	void pauseCallBack(Ref *ref);

	//2016-08-20
	MenuItem *bgmSwitchBtn;
	bool bgmSwitch;
	void pauseBGM(Ref *ref);
};

#endif // __GameLayer_H__

