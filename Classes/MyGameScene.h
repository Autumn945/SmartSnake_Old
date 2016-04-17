#pragma once
#include "cocos2d.h"
#include "GameMap.h"
#include "SystemHeader.h"

class GameMap;
class Snake;

USING_NS_CC;
using namespace std;
class MyGame : public Layer {
private:
	TMXLayer* food_layer;
	Sprite* progress[foods_num];
	float progress_length;
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
	int remain_num[foods_num];
	float cooldown[foods_num], current_cooldown[foods_num];
	static const int max_heart = 5, max_pause_n = 5;
	DEFINE_VAR_GET_ADD(int, score);
	DEFINE_VAR_GET_ADD(int, heart);
	DEFINE_VAR_GET_ADD(int, pause_n);

	static Scene* createScene(int mission_id);
	static MyGame* create(int mission_id);
	virtual bool init(int mission_id);
	virtual void update(float dt);
	void update_dir();
	void game_over();
	bool isUpdate;
	int min_score, bug, flower, kill;
	Sprite *turn_1, *turn_2;
	MenuItemFont *menu_clear_dir;
	GameMap* game_map;
	vector<Snake*> snakes;
};