#include "T13Action.h"
#include "AppMarcos.h"

CCScene *T13Action::scene()
{
	CCScene * scene = CCScene::create();
	T13Action * layer = T13Action::create();
	scene->addChild(layer);
	return scene;
}
bool T13Action::init()
{
	TBack::init();

	Sprite *hero1 = Sprite::create("hero1_001.png");
	Sprite *hero2 = Sprite::create("hero2_001.png");
	hero1->setPosition(Point(100, 80));
	hero2->setPosition(Point(100, 240));
	addChild(hero1);
	//addChild(hero2);

	Array *array_hero = Array::create();
	array_hero->retain();
	array_hero->addObject(hero2);

	//Action

	//Place *place = Place::create(Point(200, 160));//移动动作
	
	//FlipX *flipx = FlipX::create(true);//对称动作
	//FlipY *flipy = FlipY::create(true);
	
	//Show *show = Show::create();//显示动作
	//Hide *hide = Hide::create();//隐藏动作
	DelayTime *dt = DelayTime::create(0.5);//延迟动作

	//Sequence *seq = Sequence::create(show, dt, hide, dt, show, NULL);
	
	ToggleVisibility *tv = ToggleVisibility::create();//反显隐动作

	MoveTo *moveto = MoveTo::create(2, Point(200, 160));
	MoveBy *moveby = MoveBy::create(2, Point(300, 160));

	JumpTo *jumpto = JumpTo::create(2, Point(400, 160), 100, 5);
	JumpBy *jumpby = JumpBy::create(2, Point(300, 0), 100, 5);
	JumpBy *jumpby1 = JumpBy::create(2, Point(0, 0), 100, 5);

	//贝塞尔曲线
	ccBezierConfig bezconfig;
	bezconfig.controlPoint_1 = Point(200, 300);
	bezconfig.controlPoint_2 = Point(300, 20);
	bezconfig.endPosition = Point(400, 160);
	BezierTo *bezto = BezierTo::create(2, bezconfig);
	BezierBy *bezby = BezierBy::create(2, bezconfig);
	
	ScaleTo *scaleTo = ScaleTo::create(2, 2);
	ScaleBy *scaleBy = ScaleBy::create(2, 2);
	
	//旋转
	RotateTo *roTo = RotateTo::create(2, 30);
	RotateBy *roBy = RotateBy::create(2, 30);
	//扭曲
	SkewTo *skewTo = SkewTo::create(2, 20, 30);
	SkewBy *skewBy = SkewBy::create(2, 20, 30);
	//褪色
	TintTo *tintTo = TintTo::create(2, 123, 123, 123);
	TintBy *tintBy = TintBy::create(2, 123, 123, 123);
	//透明渐变
	FadeIn *fadeIn = FadeIn::create(2);
	FadeOut *fadeOut = FadeOut::create(2);
	FadeTo *fadeTo = FadeTo::create(2, 120 /*透明度*/);

	//CardinalSpline
	PointArray *pArray = PointArray::create(6);
	pArray->addControlPoint(ccp(100, 160));
	pArray->addControlPoint(ccp(200, 160));
	pArray->addControlPoint(ccp(200, 250));
	pArray->addControlPoint(ccp(300, 250));
	pArray->addControlPoint(ccp(300, 160));
	pArray->addControlPoint(ccp(450, 160));
	CardinalSplineTo *cardinalTo = CardinalSplineTo::create(2, pArray, 1);
	CardinalSplineTo *cardinalTo_rev = cardinalTo->reverse();

	//并行动作
	Blink *blink = Blink::create(5, 10);
	Spawn *spawn = Spawn::create(Sequence::create(cardinalTo, cardinalTo_rev, NULL),
								 blink,
								 NULL);

	//缓冲动作
	MoveBy *moveBy1 = MoveBy::create(3, Point(300, 0));
	MoveBy *moveBy2 = MoveBy::create(3, Point(300, 0));
	//加速
	EaseExponentialIn *easeIn = EaseExponentialIn::create(moveBy2);
	//减速
	EaseExponentialOut *easeOut = EaseExponentialOut::create(moveBy2);

	CallFunc *func = CallFunc::create(this,
									  callfunc_selector(T13Action::funcCallBack));
	CallFuncN *funcN = CallFuncN::create(this,
										 callfuncN_selector(T13Action::funcNCallBack));
	CCCallFuncND *funcND = CCCallFuncND::create(this,
												callfuncND_selector(T13Action::funcNDCallBack),
												(void*)"30");
	CCCallFuncO *funcO = CCCallFuncO::create(this,
											 callfuncO_selector(T13Action::funcOCallBack),
											 array_hero);

	
	Sequence *seq = Sequence::create(moveBy2, funcO, NULL);

	hero1->runAction(seq);

	return true;
}

void T13Action::draw(Renderer *renderer, const Mat4 &transform, uint32_t flags)
{
	_customCommand.init(_globalZOrder, transform, flags);
	_customCommand.func = CC_CALLBACK_0(T13Action::onDraw, this, transform, flags);
	renderer->addCommand(&_customCommand);
}

void T13Action::onDraw(const Mat4 &transform, uint32_t flags)
{
	/*
	ccDrawCubicBezier(Point(100, 160), Point(200, 300),
					  Point(300, 20), Point(400, 160),
					  100);
	
	PointArray *pArray = PointArray::create(6);
	pArray->addControlPoint(ccp(100, 160));
	pArray->addControlPoint(ccp(200, 160));
	pArray->addControlPoint(ccp(200, 250));
	pArray->addControlPoint(ccp(300, 250));
	pArray->addControlPoint(ccp(300, 160));
	pArray->addControlPoint(ccp(450, 160));
	ccDrawCardinalSpline(pArray, 1, 100);
	*/
}

void T13Action::funcCallBack()
{
	CCLOG("action over");
}
void T13Action::funcNCallBack(Node *node /*动作节点*/)
{
	Sprite *spr = (Sprite*)node;
	spr->removeFromParentAndCleanup(true);
}
void T13Action::funcNDCallBack(Node *node, void *data)
{
	Sprite *spr = (Sprite*)node;
	CCString *str = CCString::createWithFormat("%s", (char*)data);
	spr->setRotation(str->floatValue());
}
void T13Action::funcOCallBack(Ref *ref)
{
	Array *array = (Array*)ref;
	Sprite *spr = (Sprite*)array->getObjectAtIndex(0);
	addChild(spr);
}