#include "T12UI.h"
#include "AppMarcos.h"
#include "SimpleAudioEngine.h"


CCScene *T12UI::scene()
{
	CCScene * scene = CCScene::create();
	T12UI * layer = T12UI::create();
	scene->addChild(layer);
	return scene;
}
bool T12UI::init()
{
	TBack::init();
	
#if 0
	ControlSlider * slider = ControlSlider::create("sliderTrack.png",
													   "sliderProgress.png",
													   "sliderThumb.png");

	slider->setPosition(ccp(winSize.width / 2, winSize.height / 2 - 30));
	addChild(slider);
	slider->setMaximumValue(50);
	slider->setMinimumValue(0);

	//slider->setMaximumAllowedValue(1.5f);
	//slider->setMinimumAllowedValue(0.9f);

	slider->setValue(0);
	CCString *str = CCString::createWithFormat("%.2g", slider->getValue());

	atlas = LabelTTF::create(str->getCString(), "Courier New", 30);
	atlas->setAnchorPoint(ccp(0.5, 0.5));
	atlas->setScale(2.0f);
	atlas->setPosition(ccp(winSize.width / 2, winSize.height / 2 + 30));
	addChild(atlas);

	slider->addTargetWithActionForControlEvents(this,
												cccontrol_selector(T12UI::sliderCallBack),
												Control::EventType::VALUE_CHANGED);

	using namespace CocosDenshion;
	SimpleAudioEngine::getInstance()->preloadBackgroundMusic("sao-courage.mp3");

	Sprite *mask = CCSprite::create("switch-mask.png");
	Sprite *on = CCSprite::create("switch-on.png");
	Sprite *off = CCSprite::create("switch-off.png");
	Sprite *thumb = CCSprite::create("switch-thumb.png");
	ControlSwitch *sw = ControlSwitch::create(mask, on, off, thumb, (Label *)LabelTTF::create("On", "Courier New", 16), (Label*)LabelTTF::create("Off", "Courier New", 16));
	sw->setPosition(Point(winSize.width / 2, winSize.height / 2));
	sw->setOn(false);
	addChild(sw);
	
	sw->addTargetWithActionForControlEvents(this,
											cccontrol_selector(T12UI::switchCallBack),
											Control::EventType::VALUE_CHANGED);


	//Sprite *spr = Sprite::create("scale9.png");
	//spr->setPosition(Point(winSize.width / 2, winSize.height / 2));
	//spr->setScaleX(5);
	//spr->setScaleY(2);
	//addChild(spr);

	//Scale9Sprite *s9spr = Scale9Sprite::create("scale9.png", CCRectMake(0,0,116,102),CCRectMake(8,8,100,86));
	Scale9Sprite *s9spr = Scale9Sprite::create("scale9.png");
	s9spr->setPosition(Point(winSize.width / 2, winSize.height / 2));
	addChild(s9spr);
	s9spr->setPreferredSize(CCSize(450, 200));
#endif

	Scale9Sprite *buttonbg = Scale9Sprite::create("button_normal.png");
	Scale9Sprite *buttonlight = Scale9Sprite::create("button_click.png");
	LabelTTF *ttf = LabelTTF::create("CLICK", "Î¢ÈíÑÅºÚ", 40);

	ControlButton *button = ControlButton::create(ttf, buttonbg);
	button->setPosition(Point(winSize.width / 2, winSize.height / 2));
	button->setBackgroundSpriteForState(buttonlight, Control::State::HIGH_LIGHTED);
	addChild(button);

	//button->addTargetWithActionForControlEvents(this, xor, X);

	return true;
}

void T12UI::sliderCallBack(Ref* sender, Control::EventType event)
{
	ControlSlider * slider = (ControlSlider *)sender;
	CCString *str = CCString::createWithFormat("%.2g", slider->getValue());
	atlas->setString(str->getCString());
}

void T12UI::switchCallBack(Ref* sender, Control::EventType event)
{
	ControlSwitch *sw = (ControlSwitch *)sender;
	if(sw->isOn())
	{
		CCLOG("Switch is ON.");
		CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic("sao-courage.mp3");
	}
	else
	{
		CCLOG("Switch is OFF.");
		CocosDenshion::SimpleAudioEngine::getInstance()->stopBackgroundMusic();
	}
}