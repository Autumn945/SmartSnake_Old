#pragma once
#include "SystemHeader.h"

using namespace std;
USING_NS_CC;

class Snake : public Node{
public:
	DEFINE_VAR_GET_SET(queue<Sprite*>*, snake_nodes);
	DEFINE_VAR_GET(queue<DIRECTION>*, turn_list);
	DEFINE_VAR_GET(DIRECTION, current_dir);
	DEFINE_VAR_GET(pii, position);
	DEFINE_VAR_GET(string, image);
	DEFINE_VAR_GET(bool, is_dying);
	DEFINE_VAR_GET(bool, is_died);
	DEFINE_VAR_GET(int, time_stamp);
	DEFINE_VAR_GET_ADD(int, food);
	DEFINE_VAR_GET_ADD(int, speed);
	DEFINE_VAR_GET_ADD(int, step);

	Snake(ValueMap vm);
	bool init();
	~Snake();
	static Snake* create(ValueMap vm);
	int get_length();
	bool go_ahead();
	bool new_tail();
	bool new_head();
	bool check();
	bool turn(DIRECTION dir);
	int get_tail_time_stamp();

};

