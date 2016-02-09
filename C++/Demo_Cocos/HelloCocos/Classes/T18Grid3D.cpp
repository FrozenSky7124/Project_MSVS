#include "T18Grid3D.h"
#include "AppMarcos.h"

CCScene *T18Grid3D::scene()
{
	CCScene * scene = CCScene::create();
	T18Grid3D * layer = T18Grid3D::create();
	scene->addChild(layer);
	return scene;
}
bool T18Grid3D::init()
{
	TBack::init();

	Sprite *jx3logo = Sprite::create("jx3logo.png");
	jx3logo->setPosition(winSize.width / 2, winSize.height / 2);
	jx3logo->setScale(0.8f);
	Sprite *jx3bg = Sprite::create("bg_001.jpg");
	jx3bg->setPosition(winSize.width / 2, winSize.height / 2);

	//创建NodeGrid承载Sprite对象
	auto nodeGrid = NodeGrid::create();
	nodeGrid->addChild(jx3bg);
	nodeGrid->addChild(jx3logo);
	addChild(nodeGrid);

	//Grid3D
	auto *flipX3D = FlipX3D::create(2);
	auto *flipY3D = FlipY3D::create(2);
	auto *shaky = Shaky3D::create(5, Size(40, 40), 10, true);
	auto *wave = Waves::create(2, Size(20, 20), 30, 30, true, true);
	auto *fadeoutbl = CCFadeOutBLTiles::create(2, Size(20, 20));//网格化淡出
	auto *jumptile = JumpTiles3D::create(20, Size(5, 5), 10, 10);

	//Other Effect3D API on http://frozensky.sinaapp.com

	nodeGrid->runAction(jumptile);

	return true;
}