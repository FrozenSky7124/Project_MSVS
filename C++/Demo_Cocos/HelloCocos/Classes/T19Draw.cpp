#include "T19Draw.h"
#include "AppMarcos.h"

CCScene *T19Draw::scene()
{
	CCScene * scene = CCScene::create();
	T19Draw * layer = T19Draw::create();
	scene->addChild(layer);
	return scene;
}
bool T19Draw::init()
{
	TBack::init();

	return true;
}

//重写cocos2d-x 3.x draw方法：回调onDraw函数进行OpenGL绘图
void T19Draw::draw(Renderer *renderer, const Mat4 &transform, uint32_t flags)
{
	_customCommand.init(_globalZOrder, transform, flags);
	_customCommand.func = CC_CALLBACK_0(T19Draw::onDraw, this, transform, flags);
	renderer->addCommand(&_customCommand);
}

void T19Draw::onDraw(const Mat4 &transform, uint32_t flags)
{
	//using namespace DrawPrimitives;
	ccPointSize(10.0f);
	ccDrawColor4B(255, 0, 0, 0);
	Point p[] = { Point(100,200), Point(100,300), Point(200,100) };
	ccDrawPoints(p, 3);

	ccDrawColor4B(0, 255, 0, 0);
	glLineWidth(5);
	ccDrawLine(Point(100, 300), Point(200, 100));
	ccDrawRect(Point(100, 100), Point(400, 300));

	ccDrawColor4B(0, 0, 255, 0);
	Point poly[] = { Point(150,100),Point(200,150),Point(300,10) };
	ccDrawPoly(poly, 3, true);

	ccDrawCircle(ccp(240, 160), 60, 100, 100, false);
}
