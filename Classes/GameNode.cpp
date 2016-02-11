#include "GameNode.h"

USING_NS_CC;

int GameNode::game_width;
int GameNode::game_height;

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

pii GameNode::to_tile_map_pos(Vec2 pos) {
	pos = pos / UNIT;
	return pii(float_to_int(pos.x), game_height - float_to_int(pos.y) - 1);
}
Vec2 GameNode::to_cocos_pos(pii pos) {
	pos.second = game_height - pos.second - 1;
	return Vec2(pos.first, pos.second) * UNIT;
}

GameNode * GameNode::createWithTMXFile(string file_name) {
	GameNode *pRet = new(std::nothrow) GameNode();
	if (pRet && pRet->initWithTMXFile(file_name)) {
		pRet->autorelease();
		return pRet;
	}
	else {
		delete pRet;
		pRet = nullptr;
		return nullptr;
	}
}

bool GameNode::initWithTMXFile(string file_name) {
	if (!TMXTiledMap::initWithTMXFile(file_name)) {
		log("initWithTMXFile %s failed!", file_name.c_str());
		return false;
	}
	log("GameNode init");
	game_width = float_to_int(this->getMapSize().width);
	game_height = float_to_int(this->getMapSize().height);
	player = new(nothrow) vector<Snake*>();
	AI = new(nothrow) vector<Snake*>();

	this->setAnchorPoint(Vec2(0.5, 1));
	this->setPosition(Vec2(origin.x + visible_size.width / 2, origin.y + visible_size.height));

	snake_map = new(nothrow) set<Sprite*>*[game_width]();
	for (int i = 0; i < game_width; i++) {
		snake_map[i] = new(nothrow) set<Sprite*>[game_height]();
	}
	hole_map = new(nothrow) mp_pii_pii();

	if (this->getLayer("wall") == NULL) {
		log("layer wall of tile_map has not found!");
	}
	if (this->getLayer("grass") == NULL) {
		log("layer grass of tile_map has not found!");
	}
	else {
		this->getLayer("grass")->setLocalZOrder(1);
	}

	auto hole_obj_group = this->getObjectGroup("hole_objs");
	if (hole_obj_group) {
		log("object hole_objs of tile_map has not found!");
	}
	else {
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
	}

	auto snake_obj_group = this->getObjectGroup("snake_objs");
	CCASSERT(snake_obj_group, "!!!!!!!!!!!!!!!!!!!!snake_objs has not defined!");
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
		this->addChild(sp);
	}
	auto label = Label::createWithSystemFont("score: 0", "Arial", DEFAULT_LABEL_FONT_SIZE);
	label->setAnchorPoint(Vec2(0.5, 0));
	label->setPosition(visible_size.width / 2, 0);
	label->setName("score");
	this->addChild(label);
	scheduleUpdate();
	return true;
}

void GameNode::update(float dt) {
	for (auto snk : *player) {
		snk->go_ahead();
	}
	int score = 0;
	for (auto snk : *player) {
		snk->check();
		score += snk->get_score();
	}
	auto label = (Label*)this->getChildByName("score");
	label->setString("score: " + Value(score).asString());
}


void dfs_points(TMXLayer *wall, set<Sprite*> **mp, TMXLayer *food, vector<pii> &ret, set<pii> &vis, pii now, int dir) {
	if (vis.count(now) > 0) {
		return;
	}
	vis.insert(now);
	if (wall != NULL && wall->getTileGIDAt(Vec2(now.first, now.second)) > 0) {
		return;
	}
	if (!mp[now.first][now.second].empty()) {
		return;
	}
	if (food != NULL && food->getTileGIDAt(Vec2(now.first, now.second)) > 0) {
		return;
	}
	for (int i = 0; i < 4; i++) {
		if (abs(i - dir) == 2) {
			continue;
		}
		auto x = (now.first + dir_vector[i].first + GameNode::game_width) % GameNode::game_width;
		auto y = (now.second + dir_vector[i].second + GameNode::game_height) % GameNode::game_height;
		dfs_points(wall, mp, food, ret, vis, pii(x, y), i);
	}
	ret.push_back(now);
}

vector<pii> GameNode::get_accessible_points(pii begin, int dir) {
	vector<pii> ret;
	set<pii> vis;
	auto wall = this->getLayer("wall");
	auto mp = this->get_snake_map();
	auto food = this->getLayer("food");
	for (int i = 0; i < 4; i++) {
		if (abs(i - dir) == 2) {
			continue;
		}
		auto x = (begin.first + dir_vector[i].first + game_width) % game_width;
		auto y = (begin.second + dir_vector[i].second + game_height) % game_height;
		dfs_points(wall, mp, food, ret, vis, pii(x, y), i);
	}
	for (auto p : ret) {
		log("---> %d, %d", p.first, p.second);
	}
	return ret;
}
