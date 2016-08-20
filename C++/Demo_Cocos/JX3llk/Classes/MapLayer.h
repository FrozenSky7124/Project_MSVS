#ifndef __MapLayer_H__
#define __MapLayer_H__

#include "cocos2d.h"
#include "MapNode.h"

USING_NS_CC;

class MapLayer : public LayerColor
{
public:

	virtual bool init(int level);

	//CREATE_FUNC(MapLayer);
	static MapLayer * create(int level, int score);

	int _level;
	int _score;
	int _remainNum;
	CCArray *totalArray;
	CCArray *realArray;

	int getScore();
	int getLevel();

private:
	void initUI();
	void initMapNode(int level);
	void initTouch();
	void debug();

	virtual bool onTouchBegan(Touch *touch, Event *event);
	virtual void onTouchEnded(Touch *touch, Event *event);
	MapNode* onMapNode(MapNode **temp, Touch *touch);

	bool isSameIndex(MapNode *pre, MapNode *cur);
	bool match(MapNode* pre, MapNode* cur);
	bool matchDirect(MapNode *pre, MapNode *cur);
	bool matchOnecorner(MapNode *pre, MapNode *cur);
	bool matchTwocorner(MapNode *pre, MapNode *cur);
	void clearNode(MapNode *pre, MapNode *cur);
	void linkNode(MapNode *pre, MapNode *cur);
	void clearLinkNode(MapNode *pre, MapNode *cur);
	void clearAllLink(float dt);
	MapNode * PointToMapNode(int x, int y);

	//折点信息（用于连线）
	MapNode *linkedp;
	MapNode *linkedc;
	MapNode *z1;
	MapNode *z2;

	MapNode *_preNode;
	MapNode *_curNode;
};

#endif // __MapLayer_H__

