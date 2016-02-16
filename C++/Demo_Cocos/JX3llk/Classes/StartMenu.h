#ifndef __StartMenu_H__
#define __StartMenu_H__

#include "cocos2d.h"

USING_NS_CC;

class StartMenu : public Layer
{
public:
	static Scene* createScene();

	virtual bool init();
	
	CREATE_FUNC(StartMenu);

	void startGameCallBack(Ref *ref);
	void quitGameCallBack(Ref *ref);
	void rankGameCallBack(Ref *ref);

protected:

	void initUI();
	void debug();
	void update(float delta);

	Vector<Sprite*> * spriteVector;
};

#endif // __StartMenu_H__