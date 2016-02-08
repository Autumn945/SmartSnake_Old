#pragma once
#include "SystemHeader.h"
USING_NS_CC;
using namespace std;

class Option : public Layer {
public:
	static Scene* createScene(string name);
	virtual bool init();
	static Option* create(string name);
};

