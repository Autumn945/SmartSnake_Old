#pragma once
#include "cocos2d.h"
#include "SystemHeader.h"
#include "GameMap.h"

USING_NS_CC;
class Mission: public Sprite {
public:
	DEFINE_VAR_GET(GameMap*, game_map);
	DEFINE_VAR_GET(int, score);
	int flower[3], success, challenge;

	int get_flower();
	static Mission* create(int id);
	virtual bool init(int id);
};

