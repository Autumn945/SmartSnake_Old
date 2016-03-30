#pragma once
#include "cocos2d.h"
#include "Snake.h"

using namespace std;
USING_NS_CC;

class SmartSnake: public Snake {
public:
	Snake::SnakeType type;
protected:
	pii target;
public:
	virtual bool init(string name, GameMap* game_map);
	static SmartSnake* create(string name, GameMap* game_map);
	virtual bool eat(int gid);
	virtual bool go_die();
	virtual bool go_ahead();
	virtual SnakeType get_type();
	bool act();

};

