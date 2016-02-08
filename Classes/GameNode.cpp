#include "GameNode.h"

USING_NS_CC;

TMXTiledMap* GameNode::tile_map;
set<Sprite*>** GameNode::snake_map;
mp_pii_pii* GameNode::hole_map;

GameNode::~GameNode() {
	delete player;
	delete AI;
	delete hole_map;
	for (int i = 0; i < game_width; i++) {
		delete[] snake_map[i];
	}
	delete[] snake_map;
	log("deleted a GameNode");
}

bool GameNode::init() {
	if (!Node::init()) {
		return false;
	}
	log("GameNode init");
	this->setColor(Color3B::RED);
	player = new(nothrow) vector<Snake*>();
	AI = new(nothrow) vector<Snake*>();

	this->setContentSize(Size(UNIT * game_width, UNIT * game_height));
	this->setAnchorPoint(Vec2(0, 1));
	this->setPosition(Vec2(origin.x, origin.y + visible_size.height));

	snake_map = new(nothrow) set<Sprite*>*[game_width]();
	for (int i = 0; i < game_width; i++) {
		snake_map[i] = new(nothrow) set<Sprite*>[game_height]();
	}

	hole_map = new(nothrow) mp_pii_pii();
	tile_map = TMXTiledMap::create("0.tmx");
	if (tile_map == NULL) {
		log("0.tmx has not found!");
		return false;
	}
	if (tile_map->getLayer("wall") == NULL) {
		log("layer wall of tile_map has not found!");
	}
	tile_map->setAnchorPoint(Vec2::ZERO);
	tile_map->setPosition(Vec2::ZERO);
	if (tile_map->getLayer("grass") == NULL) {
		log("layer grass of tile_map has not found!");
	}
	else {
		tile_map->getLayer("grass")->setLocalZOrder(1);
	}
	this->addChild(tile_map);

	auto hole_obj_group = tile_map->getObjectGroup("hole_objs");
	if (hole_obj_group == NULL) {
		log("object hole_objs of tile_map has not found!");
		return false;
	}
	auto hole_objs = hole_obj_group->getObjects();
	for (auto _hole : hole_objs) {
		auto hole = _hole.asValueMap();
		auto pos = to_tile_map_pos(Vec2(hole["x"].asFloat(), hole["y"].asFloat()));
		if (hole.count("to") == 0) {
			log("property \"to\" of hole %s has not found!", hole["name"].asString().c_str());
			continue;
		}
		auto to_hole = hole_obj_group->getObject(hole["to"].asString());
		if (to_hole.size() == 0) {
			log("hole %s has not found!", hole["to"].asString().c_str());
			continue;
		}
		auto to = to_tile_map_pos(Vec2(to_hole["x"].asFloat(), to_hole["y"].asFloat()));
		(*hole_map)[pos] = to;
	}


	auto snake_obj_group = tile_map->getObjectGroup("snake_objs");
	if (snake_obj_group == NULL) {
		log("!!!!!!!!!!!!!!!!!!!!snake_objs has not defined!");
		return false;
	}
	auto snake_objs = snake_obj_group->getObjects();
	for (auto _snake : snake_objs) {
		auto snake = _snake.asValueMap();
		auto sp = Snake::create(snake);
		if (snake["type"].asString() == "player") {
			player->push_back(sp);
		}
		else {
			AI->push_back(sp);
		}
		tile_map->addChild(sp);
	}

	schedule([this](float dt) {
		for (auto snk : *player) {
			snk->go_ahead();
		}
		for (auto snk : *player) {
			snk->check();
		}
	}, "update");

	return true;
}
