#pragma once
#include "SystemHeader.h"

class GameNode;

USING_NS_CC;
using namespace std;
class MyGame : public Layer {
public:
	static Scene* createScene();
	virtual bool init();
	CREATE_FUNC(MyGame);

	Sprite *control;
	GameNode *game;

	//test
	Label *top_label;
};

