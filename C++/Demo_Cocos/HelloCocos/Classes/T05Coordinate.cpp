#include "T05Coordinate.h"
#include "AppMarcos.h"

CCScene * T05Coordinate::scene()
{
	CCScene *scene = CCScene::create();
	T05Coordinate *layer = T05Coordinate::create();
	scene->addChild(layer);
	return scene;
}

bool T05Coordinate::init()
{
	TBack::init();
	/*
		GL������ϵ�����½�Ϊԭ��
		UI������ϵ�����Ͻ�Ϊԭ��

		����������ϵ���Ǵ��ڵ�������ϵ��GL������ϵ��ԭ���Ǵ��ڵ����½ǡ�
		�ڵ�������ϵ����Node��������ϵ��GL������ϵ��ԭ���ǽڵ�����½ǡ�
	*/
	CCLayerColor *layercolor = CCLayerColor::create(ccc4(0, 0, 255, 255), winSize.width / 2, winSize.height / 2);
	this->addChild(layercolor);
	layercolor->setAnchorPoint(ccp(0, 0));
	layercolor->setPosition(ccp(200, 200));
	

	hero02_tx = CCSprite::create("hero2_tx.png");
	//this->addChild(hero02_tx);
	layercolor->addChild(hero02_tx);
	hero02_tx->setPosition(ccp(100, 100));

	CCPoint pt_hero02_tx = layercolor->convertToWorldSpace(hero02_tx->getPosition());
	CCLOG("hero02_tx.x=%f hero02_tx.y=%f", pt_hero02_tx.x, pt_hero02_tx.y);

	//���봥��
	setTouchEnabled(true);

	CCMoveBy *by1 = CCMoveBy::create(2, ccp(200, 0));
	CCMoveBy *by2 = by1->reverse();
	CCSequence *seq = CCSequence::create(by1, by2, NULL);
	layercolor->runAction(CCRepeatForever::create(seq));

	return true;
}

void T05Coordinate::onEnter()
{
	CCLayer::onEnter();

	// Register Touch Event
	auto listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true);

	listener->onTouchBegan = CC_CALLBACK_2(T05Coordinate::onTouchBegan, this);
	listener->onTouchMoved = CC_CALLBACK_2(T05Coordinate::onTouchMoved, this);
	listener->onTouchEnded = CC_CALLBACK_2(T05Coordinate::onTouchEnded, this);

	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}

void T05Coordinate::onExit()
{
	CCLayer::onExit();
}

bool T05Coordinate::onTouchBegan(CCTouch * touch, CCEvent * event)
{
	CCPoint pGL = touch->getLocation();
	CCPoint pUI = touch->getLocationInView();

	//��ȡ������ӿ�����
	CCRect rect = hero02_tx->boundingBox();
	//�жϵ��λ���Ƿ�Ϊ���������
	if (rect.containsPoint(hero02_tx->getParent()->convertToNodeSpace(pGL)))
	{
		pOffset = hero02_tx->convertToNodeSpaceAR(touch->getLocation());
		CCLOG("onTouchBegan id = %d, x = %.2f, y = %.2f onGL", touch->getID(), pGL.x, pGL.y);
		CCLOG("onTouchBegan id = %d, x = %.2f, y = %.2f onUI", touch->getID(), pUI.x, pUI.y);
	}
	return true;
}

void T05Coordinate::onTouchMoved(CCTouch * touch, CCEvent * event)
{
	//CCLOG("onTouchMoved id = %d, x = %.2f, y = %.2f", touch->getID(), touch->getLocation().x, touch->getLocation().y);
	CCPoint pGL = hero02_tx->getParent()->convertToNodeSpace(touch->getLocation());
	hero02_tx->setPosition(pGL-pOffset);
}

void T05Coordinate::onTouchEnded(CCTouch * touch, CCEvent * event)
{

}