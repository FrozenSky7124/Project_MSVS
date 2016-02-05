#ifndef __T13Action_H__
#define __T13Action_H__

#include "cocos2d.h"
#include "TBack.h"

USING_NS_CC;

class T13Action :public TBack
{
public:
	static CCScene * scene();
	CREATE_FUNC(T13Action);
	bool init();

	//÷ÿ–¥draw∑Ω∑®
	virtual void draw(Renderer *renderer, const Mat4 &transform, uint32_t flags) override;
	void onDraw(const Mat4 &transform, uint32_t flags);
	void funcCallBack();
	void funcNCallBack(Node *node);
	void funcNDCallBack(Node *node, void *data);
	void funcOCallBack(Ref *ref);
protected:
	CustomCommand _customCommand;
};


#endif