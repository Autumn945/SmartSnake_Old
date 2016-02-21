#pragma once
#include "SystemHeader.h"

USING_NS_CC;
using namespace std;
class GameMenu : public Layer {
public:
	static Scene* createScene();
	virtual bool init();
	CREATE_FUNC(GameMenu);
	GameMap* world;
	Snake* player;
	map<pii, string> game_map;
};

