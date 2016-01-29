#include "T07Schedule.h"
#include "AppMarcos.h"

CCScene * T07Schedule::scene()
{
	CCScene *scene = CCScene::create();
	T07Schedule *layer = T07Schedule::create();
	scene->addChild(layer);
	return scene;
}

bool T07Schedule::init()
{
	CCLayer::init();

	//֡ѭ����ʱ��-����update()����
	//scheduleUpdate();
	//֡ѭ����ʱ��-ֹͣ
	//unscheduleUpdate();
	
	//һ���Զ�ʱ��
	//scheduleOnce(schedule_selector(T07Schedule::mySchedule), 2);
	
	//֡ѭ����ʱ��-�Զ���ص�����
	//schedule(schedule_selector(T07Schedule::mySchedule));
	//schedule(schedule_selector(T07Schedule::mySchedule), 3);
	//schedule(schedule_selector(T07Schedule::mySchedule), 3, 10, 4);

	CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("hero0_move.plist");
	CCSprite *hero0 = CCSprite::createWithSpriteFrameName("hero0_01.png");
	hero0->setPosition(ccp(winSize.width / 2, winSize.height / 2));
	this->addChild(hero0);
	hero0->setTag(100);

	//Scheduleʵ�־����˶�
	schedule(schedule_selector(T07Schedule::heroMove));

	//��Ӧ��ֹͣ����
	//unschedule(schedule_selector(T07Schedule::heroMove));

	return true;
}

void T07Schedule::update(float dt)
{
	CCLOG("%g", dt);
}

void T07Schedule::mySchedule(float dt)
{
	CCLOG("%g", dt);
}

void T07Schedule::heroMove(float dt)
{
	float vx = 300 / 3;
	CCSprite *spr = (CCSprite*)getChildByTag(100);
	static bool flag = true;
	if (spr->getPositionX() >= (winSize.width / 2 + 300))
	{
		flag = false;
	}
	if (spr->getPositionX() <= (winSize.width / 2))
	{
		flag = true;
	}

	if (flag)
	{
		spr->setPositionX(spr->getPositionX() + vx*dt);
		CCLOG("%.2f", spr->getPositionX());
	}
	else
	{
		spr->setPositionX(spr->getPositionX() - vx*dt);
		CCLOG("%.2f", spr->getPositionX());
	}

	//CCLOG("%g", dt);
}
