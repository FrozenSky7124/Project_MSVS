#include "T22UserDefault.h"
#include "AppMarcos.h"

CCScene *T22UserDefault::scene()
{
	CCScene * scene = CCScene::create();
	T22UserDefault * layer = T22UserDefault::create();
	scene->addChild(layer);
	return scene;
}
bool T22UserDefault::init()
{
	TBack::init();

	UserDefault::getInstance()->setIntegerForKey("integer", 100);
	UserDefault::getInstance()->setStringForKey("string", "ooo");
	UserDefault::getInstance()->flush();
	auto value = UserDefault::getInstance()->getIntegerForKey("integer", 120);
	auto path = UserDefault::getInstance()->getXMLFilePath();
	CCLOG("%s", path.c_str());

	//Dictionary
	Dictionary *dic = Dictionary::create();
	dic->retain();
	String *value1 = String::create("100");
	String *value2 = String::create("200");
	dic->setObject(value1, "key1");
	dic->setObject(value2, "key2");

	String *str = (String*)dic->objectForKey("key1");
	CCLOG("%s", str->getCString());
	
	return true;
}
