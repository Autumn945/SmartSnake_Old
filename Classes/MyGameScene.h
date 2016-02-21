#pragma once
#include "SystemHeader.h"

class GameMap;
class Snake;

USING_NS_CC;
using namespace std;
class MyGame : public Layer {
public:
	static Scene* createScene(string file_name);
	static string map_file_name;
	virtual bool init();
	CREATE_FUNC(MyGame);
	virtual void update(float dt);

	GameMap* game_map;
	Snake* player;
	DEFINE_VAR_GET_ADD(int, speed);
	DEFINE_VAR_GET_ADD(int, step);
};