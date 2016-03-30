#pragma once
#include "cocos2d.h"
#include "GameMap.h"
#include "SystemHeader.h"

class GameMap;
class Snake;

USING_NS_CC;
using namespace std;
class MyGame : public Layer {
public:
	enum FOOD {
		food_green_apple = 1
		, food_red_apple
		, food_bird
		, food_cola
		, food_bug
		, food_flower
		, food_heart
		, food_shit
	};
	int cooldown[foods_num];
	float current_cooldown[foods_num];
	DEFINE_VAR_GET_ADD(int, score);

	static Scene* createScene(int mission_id);
	static MyGame* create(int mission_id);
	virtual bool init(int mission_id);
	virtual void update(float dt);
	void game_over();

	int min_score, bug, flower;
	GameMap* game_map;
	vector<Snake*> snakes;
};