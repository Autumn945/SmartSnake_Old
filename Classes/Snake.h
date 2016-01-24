#pragma once
#include "SystemHeader.h"

using namespace std;
USING_NS_CC;

class Snake {
public:
	struct SnakeNode {
		int x, y, type, dir;
		SnakeNode(int _x, int _y, int _t, int _d) :x(_x), y(_y), type(_t), dir(_d) {};
	};
	enum SnakeNodeType {
		body_turn_left = -1, body, body_turn_right, unknown
	};
	//for self
	DEFINE_VAR_GET_SET(queue<SnakeNode*>*, snake_nodes);
	DEFINE_VAR_GET_SET(int, food_cache);
	DEFINE_VAR_GET_SET(int, next_turn);
	DEFINE_VAR_GET_SET(float, add_speed);
	DEFINE_VAR_GET_SET(float, head_speed);
	DEFINE_VAR_GET_SET(float, head_step);
	DEFINE_VAR_GET_SET(float, tail_step);
	DEFINE_VAR_GET_SET(float, tail_speed);
	Snake(int x, int y, int dir);
	~Snake();
	int get_length();
	int get_current_dir();
	void update(float dt);

	Scheduler *scheduler;
	void get_next_node();

	//for draw
	DEFINE_VAR_GET(queue<SnakeNode*>*, remove_cache);
	DEFINE_VAR_GET(queue<SnakeNode*>*, add_cache);

};

