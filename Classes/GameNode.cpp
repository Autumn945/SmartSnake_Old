#include "GameNode.h"

bool GameNode::init() {
	if (!Node::init()) {
		return false;
	}
	log("game node init");
	//create node game
	width = INTEGER("game_width");
	height = INTEGER("game_height");
	this->setContentSize(Size(UNIT * width, UNIT * height));
	this->setAnchorPoint(Vec2(0, 1));
	this->setPosition(MyUtility::origin.x, MyUtility::origin.y + MyUtility::visible_size.height);
	log("set position finished");
	wall = SpriteBatchNode::create(STRING("file_wall"));
	wall->setAnchorPoint(Vec2(0, 0));
	wall->setPosition(Vec2(0, 0));
	//grass = SpriteBatchNode::create();
	hole = SpriteBatchNode::create(STRING("file_hole"));
	hole->setAnchorPoint(Vec2(0, 0));
	hole->setPosition(Vec2(0, 0));
	this->addChild(wall);
	//this->addChild(grass);
	this->addChild(hole);
	log("add child finished");
	map_wall = new bool*[width];
	map_grass = new bool*[width];
	map_hole = new bool*[width];
	for (int i = 0; i < width; i++) {
		map_wall[i] = new bool[height]();
		map_grass[i] = new bool[height]();
		map_hole[i] = new bool[height]();
	}
	log("new map unit finished");
	for (int i = 0; i < width; i++) {
		map_wall[i][0] = map_wall[i][height - 1] = true;
	}
	for (int i = 0; i < height; i++) {
		map_wall[0][i] = map_wall[width - 1][i] = true;
	}
	log("wall finished");
	map_hole[0][height / 2] = map_hole[width - 1][height / 2] = true;
	map_hole[width / 2][0] = map_hole[width / 2][height - 1] = true;
	log("map init finished");
	this->draw_unit();
	return true;
}

void GameNode::draw_unit() {
	log("draw hole");
	for (int i = 0; i < width; i++) {
		for (int j = 0; j < height; j++) {
			if (map_hole[i][j]) {
				map_wall[i][j] = false;
				auto map_hole = Sprite::create(STRING("file_wall"));
				map_hole->setAnchorPoint(Vec2(0, 0));
				map_hole->setPosition(Vec2(i * UNIT, j * UNIT));
				hole->addChild(map_hole);
			}
		}
	}
	log("draw wall");
	for (int i = 0; i < width; i++) {
		for (int j = 0; j < height; j++) {
			if (map_wall[i][j]) {
				auto wall_unit = Sprite::create(STRING("file_wall"));
				wall_unit->setAnchorPoint(Vec2(0, 0));
				wall_unit->setPosition(Vec2(i * UNIT, j * UNIT));
				wall->addChild(wall_unit);
			}
		}
	}
}