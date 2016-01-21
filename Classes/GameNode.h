#pragma once
#include "SystemHeader.h"

USING_NS_CC;
using namespace std;
class GameNode : public Node {
public:
	virtual bool init();
	void draw_unit();
	CREATE_FUNC(GameNode);

	CC_SYNTHESIZE(int, width, _width);
	CC_SYNTHESIZE(int, height, _height);
	CC_SYNTHESIZE_READONLY(int, unit, _unit);

private:
	bool **map_wall;
	bool **map_grass;
	bool **map_hole;
	SpriteBatchNode *wall, *grass, *hole;
};

