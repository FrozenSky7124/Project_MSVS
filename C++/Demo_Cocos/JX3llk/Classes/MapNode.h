#ifndef __MapNode_H__
#define __MapNode_H__

#include "cocos2d.h"

USING_NS_CC;

class MapNode : public Sprite
{
public:
	virtual bool init(int index);
	//CREATE_FUNC(MapNode);
	static MapNode * create(int index);

	void setX(int x);
	int getX();

	void setY(int y);
	int getY();

	void setIndex(int index);
	int getIndex();

	void onSelect();
	void offSelect();

	Sprite *bg;
	Sprite *onLink;

private:
	int _x;
	int _y;
	int _index; //MapNode标志位：-1表示虚空 >0表示图块
};

#endif // __MapNode_H__
