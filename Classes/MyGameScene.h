#pragma once
#include "SystemHeader.h"

class GameMap;
class Snake;

USING_NS_CC;
using namespace std;
class MyGame : public Layer {
public:
	static Scene* createScene(int mission_id);
	static MyGame* create(int mission_id);
	virtual bool init(int mission_id);
	virtual void update(float dt);
	void game_over();

	int min_score, bug, flower;
	GameMap* game_map;
	Snake* player;
	DEFINE_VAR_GET_ADD(int, speed);
	DEFINE_VAR_GET_ADD(int, step);
};