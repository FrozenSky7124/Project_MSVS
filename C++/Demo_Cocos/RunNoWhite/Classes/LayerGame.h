#ifndef __LayerGame_H__
#define __LayerGame_H__

#include "cocos2d.h"

USING_NS_CC;

class LayerGame : public Layer
{
public:
	static Scene* createScene();
	CREATE_FUNC(LayerGame);
	virtual bool init();

	bool onTouchBegan(Touch *touch, Event *event);

	void startGame();
	void addStartLineBlocks();
	void addNormalLineBlocks(int lineIndex);
	void addEndLineBlocks();
	void moveDown();
	void startTimer();
	void stopTimer();
	void update(float delta);
	

	CC_SYNTHESIZE(int, _lineCount, LineCount)
	bool showEnd;
	long startTime;
	bool isRunning;
	LabelTTF *ttf;
};

#endif // __LayerGame_H__
