#include "T21Particle.h"
#include "AppMarcos.h"

CCScene *T21Particle::scene()
{
	CCScene * scene = CCScene::create();
	T21Particle * layer = T21Particle::create();
	scene->addChild(layer);
	return scene;
}
bool T21Particle::init()
{
	TBack::init();

	ParticleExplosion *p1 = ParticleExplosion::create();


	p1->setPosition(ccp(winSize.width / 2, winSize.height / 2));
	addChild(p1);

	return true;
}
