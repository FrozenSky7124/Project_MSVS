#ifndef __GameLayer_H__
#define __GameLayer_H__

#include "cocos2d.h"
#include "MapLayer.h"

USING_NS_CC;

class GameLayer : public Layer
{
public:
	static Scene* createScene();

	//��Ϸ������Ҫ��ʼ������������ʼ��MapLayer����_map����������ͼ�鲼��
	virtual bool init();

	CREATE_FUNC(GameLayer);

	int _level; //�洢��_map�����ȡ�Ĺؿ���Ϣ
	int _score; //�洢��_map�����ȡ�ķ�����Ϣ
	int _remainNum; //�洢��_map�����ȡ�ĵ�ǰ�ؿ�ʣ��ͼ������

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

