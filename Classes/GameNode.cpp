#include "GameNode.h"

bool GameNode::init() {
	if (!Node::init()) {
		return false;
	}
	//create node game
	width = INTEGER("game_width");
	height = INTEGER("game_height");
	this->setContentSize(Size(UNIT * width, UNIT * height));
	this->setAnchorPoint(Vec2(0, 1));
	this->setPosition(MyUtility::origin.x, MyUtility::origin.y + MyUtility::visible_size.height);
	map_wall = new bool*[width];
	map_grass = new bool*[width];
	map_hole = new bool*[width];
	for (int i = 0; i < width; i++) {
		map_wall[i] = new bool[height];
		map_grass[i] = new bool[height];
		map_hole[i] = new bool[height];
	}
	for (int i = 0; i < width; i++) {
		map_wall[i][0] = map_wall[i][height - 1] = true;
	}
	for (int i = 0; i < height; i++) {
		map_wall[0][i] = map_wall[width - 1][i] = true;
	}
	return false;
}
