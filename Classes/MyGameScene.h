#pragma once
#include "SystemHeader.h"


USING_NS_CC;
using namespace std;
class MyGame : public Layer {
public:
	static Scene* createScene();
	virtual bool init();
	bool deal_with_event(string event_name);
	CREATE_FUNC(MyGame);

	Node *control;
	Sprite *turn_left, *turn_right, *turn_left_back, *turn_right_back;
	Node* game;
};

