#include "GameNode.h"

USING_NS_CC;

bool GameNode::init() {
	if (!Node::init()) {
		return false;
	}
	log("game node init");

	//create node game
	width = get_int("game_width");
	height = get_int("game_height");
	this->setContentSize(Size(UNIT * width, UNIT * height));
	this->setAnchorPoint(Vec2(0, 1));
	this->setPosition(Vec2(origin.x, origin.y + visible_size.height) + Vec2(UNIT / 2, UNIT / 2));
	log("set position finished");
	wall = SpriteBatchNode::create(get_UTF8_string("file_wall"));
	wall->setAnchorPoint(Vec2(0, 0));
	wall->setPosition(Vec2(0, 0));
	grass = SpriteBatchNode::create(get_UTF8_string("file_grass"));
	grass->setAnchorPoint(Vec2(0, 0));
	grass->setPosition(Vec2(0, 0));
	hole = SpriteBatchNode::create(get_UTF8_string("file_hole"));
	hole->setAnchorPoint(Vec2(0, 0));
	hole->setPosition(Vec2(0, 0));
	this->addChild(wall, wall_order);
	this->addChild(grass, grass_order);
	this->addChild(hole, hole_order);
	log("add child finished");
	map_wall = new bool*[width];
	map_grass = new bool*[width];
	map_hole = new bool*[width];
	map_snake = new bool*[width];
	for (int i = 0; i < width; i++) {
		map_wall[i] = new bool[height]();
		map_grass[i] = new bool[height]();
		map_hole[i] = new bool[height]();
		map_snake[i] = new bool[height]();
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

	player = new Snake(0, height / 2, DIRECTION::RIGHT);
	player->set_add_speed(3);

	for (int i = 0; i < 100; i++) {
		int x = random(1, width - 2);
		int y = random(1, height - 2);
		map_grass[x][y] = true;
	}
	for (int i = 0; i < 30; i++) {
		int x = random(1, width - 2);
		int y = random(1, height - 2);
		map_wall[x][y] = true;
	}
	log("map init finished");
	this->draw_unit();
	scheduleUpdate();
	return true;
}

void GameNode::draw_unit() {
	log("draw grass");
	for (int i = 0; i < width; i++) {
		for (int j = 0; j < height; j++) {
			if (map_grass[i][j]) {
				auto grass_unit = Sprite::create(get_UTF8_string("file_grass"));
				grass_unit->setPosition(Vec2(i * UNIT, j * UNIT));
				grass->addChild(grass_unit);
			}
		}
	}
	log("draw hole");
	for (int i = 0; i < width; i++) {
		for (int j = 0; j < height; j++) {
			if (map_hole[i][j]) {
				map_wall[i][j] = false;
				auto hole_unit = Sprite::create(get_UTF8_string("file_wall"));
				hole_unit->setPosition(Vec2(i * UNIT, j * UNIT));
				hole->addChild(hole_unit);
			}
		}
	}
	log("draw wall");
	for (int i = 0; i < width; i++) {
		for (int j = 0; j < height; j++) {
			if (map_wall[i][j]) {
				auto wall_unit = Sprite::create(get_UTF8_string("file_wall"));
				wall_unit->setPosition(Vec2(i * UNIT, j * UNIT));
				wall_unit->setRotation(random(0, 3) * 90);
				wall->addChild(wall_unit);
			}
		}
	}
}

void GameNode::draw_snake(Snake * snake) {
	while (!snake->get_add_cache()->empty() && snake->get_add_cache()->front()->type != Snake::SnakeNodeType::unknown) {
		auto node = snake->get_add_cache()->front();
		if (node->x >= 0 && node->x < width && node->y >= 0 && node->y < height) {
			map_snake[node->x][node->y] = true;
			log("draw at (%d, %d)", node->x, node->y);
		}
		Sprite *sp = NULL;
		switch (node->type) {
		case Snake::SnakeNodeType::body:
			sp = Sprite::create(get_UTF8_string("file_players_body"));
			break;
		case Snake::SnakeNodeType::body_turn_left:
			sp = Sprite::create(get_UTF8_string("file_players_body_turn"));
			sp->setFlippedX(true);
			break;
		case Snake::SnakeNodeType::body_turn_right:
			sp = Sprite::create(get_UTF8_string("file_players_body_turn"));
			break;
		default:
			break;
		}
		if (sp) {
			sp->setTag(get_tag_of_point(node->x, node->y));
			sp->setPosition(Vec2(node->x, node->y) * UNIT);
			sp->setRotation(node->dir * 90);
			this->addChild(sp);
		}
		snake->get_add_cache()->pop();
	}
}

void GameNode::update(float dt) {
	Node::update(dt);
	log("GameNode updating~~");
	player->update(dt);
	draw_snake(player);
}

int GameNode::get_tag_of_point(int x, int y) {
	return x * height + y;
}