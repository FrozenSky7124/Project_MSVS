#include "T04LayerColor.h"
#include "AppMarcos.h"

class MyLayerGradient : public CCLayerGradient
{
public:
	CREATE_FUNC(MyLayerGradient);
	bool init()
	{
		//CCLayerGradient::initWithColor(ccc4(255, 0, 0, 255), ccc4(0, 0, 255, 255));
		CCLayerGradient::initWithColor(ccc4(255, 0, 0, 255), ccc4(0, 0, 255, 255), ccp(1, 0));
		return true;
	}
};

CCScene * T04LayerColor::scene()
{
	CCScene *scene = CCScene::create();
	//T04LayerColor *layer = T04LayerColor::create();
	MyLayerGradient *layer = MyLayerGradient::create();
	TBack *layerback = TBack::create();
	scene->addChild(layer);
	scene->addChild(layerback);
	return scene;
}

bool T04LayerColor::init()
{
	CCLayerColor::initWithColor(ccc4(0, 0, 255, 150));
	CCLOG("T04LayerColor::init()");
	return true;
}