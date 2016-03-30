#pragma once
#include "cocos2d.h"
#include "MissionSprite.h"

USING_NS_CC;
using namespace std;

class GameMenu : public Layer {
public:
	int max_mission;
	Mission* select_mission;
	static Scene* createScene();
	virtual bool init();
	CREATE_FUNC(GameMenu);
};

