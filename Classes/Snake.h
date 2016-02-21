#pragma once
#include "SystemHeader.h"

using namespace std;
USING_NS_CC;

class Snake : public SpriteBatchNode{
public:
	DEFINE_VAR_GET_SET(GameMap*, game_map);
	DEFINE_VAR_GET_SET(queue<Sprite*>*, snake_nodes);
	DEFINE_VAR_GET(queue<DIRECTION>*, turn_list);
	DEFINE_VAR_GET(DIRECTION, current_dir);
	DEFINE_VAR_GET(pii, position);
	DEFINE_VAR_GET(int, score);
	DEFINE_VAR_GET(string, image);
	DEFINE_VAR_GET(bool, is_died);
	DEFINE_VAR_GET_ADD(int, food);
protected:
	pii target;
	int hunger;

public:
	Snake(ValueMap vm, GameMap* game_map);
	~Snake();
	virtual bool init();
	static Snake* create(ValueMap vm, GameMap* game_map);
	int get_length();
	bool go_ahead();
	bool new_tail();
	bool new_head();
	bool check();
	bool turn(DIRECTION dir);
	bool go_die();
	int get_tail_time_stamp();

	bool act();

};

