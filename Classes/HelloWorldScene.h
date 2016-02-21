#pragma once
#include "SystemHeader.h"

USING_NS_CC;
using namespace std;
class HelloWorld : public Layer {
public:
	static Scene* createScene();
	virtual bool init();
	CREATE_FUNC(HelloWorld);
};

