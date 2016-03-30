#pragma once
#include "cocos2d.h"

USING_NS_CC;
using namespace std;
class HelloWorld : public Layer {
public:
	static Scene* createScene();
	virtual bool init();
	CREATE_FUNC(HelloWorld);
};

