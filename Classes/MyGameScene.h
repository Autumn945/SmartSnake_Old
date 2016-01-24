#pragma once
#include "SystemHeader.h"

class GameNode;

USING_NS_CC;
using namespace std;
class MyGame : public Layer {
public:
	static Scene* createScene();
	virtual bool init();
	bool deal_with_event(string event_name);
	virtual bool onTouchBegan(Touch * touch, Event * event);
	virtual void onTouchEnded(Touch * touch, Event * event);
	bool is_tap(Node * node, Vec2 position);
	bool deal_with_touch(Vec2 position);
	virtual void update(float dt);
	CREATE_FUNC(MyGame);

	Node *control;
	Sprite *turn_left, *turn_right, *turn_left_back, *turn_right_back;
	GameNode* game;
	int pre_dir_v;

	//test
	Label *top_label;
};

