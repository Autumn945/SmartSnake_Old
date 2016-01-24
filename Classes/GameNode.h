#pragma once
#include "SystemHeader.h"

class Snake;

USING_NS_CC;
using namespace std;
class GameNode : public Node {
public:
	virtual bool init();
	void draw_unit();
	CREATE_FUNC(GameNode);
	//CC_SYNTHESIZE_READONLY(static GameNode, instance, _instance);
	DEFINE_VAR_GET_SET(int, width);
	DEFINE_VAR_GET_SET(int, height);
	DEFINE_VAR_GET_SET(Snake*, player);
	enum order {
		wall_order, hole_order, snake_body_order, snake_head_order, grass_order
	};
private:
	bool **map_wall;
	bool **map_grass;
	bool **map_hole;
	bool **map_snake;
	SpriteBatchNode *wall, *grass, *hole;
	void draw_snake(Snake *snake);
	virtual void update(float dt);
	int get_tag_of_point(int x, int y);
};

