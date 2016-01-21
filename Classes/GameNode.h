#pragma once
#include "SystemHeader.h"

USING_NS_CC;
using namespace std;
class GameNode : public Node {
public:
	virtual bool init();
	CREATE_FUNC(GameNode);

	static enum MAP_UNIT {
		SNAKE, WALL, GRASS
	};
	CC_SYNTHESIZE(int, width, _width);
	CC_SYNTHESIZE(int, height, _height);
	CC_SYNTHESIZE_READONLY(int, unit, _unit);

	vector<MAP_UNIT> *mp;
};

