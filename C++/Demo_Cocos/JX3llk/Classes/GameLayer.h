#ifndef __GameLayer_H__
#define __GameLayer_H__

#include "cocos2d.h"
#include "MapLayer.h"

USING_NS_CC;

class GameLayer : public Layer
{
public:
	static Scene* createScene();

	virtual bool init();

	CREATE_FUNC(GameLayer);

	int _level;
	int _score;
	int _remainNum;

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
};

#endif // __GameLayer_H__

