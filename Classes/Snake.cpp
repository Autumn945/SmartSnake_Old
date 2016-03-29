#include "Snake.h"

#define HEAD Rect(0, 0, UNIT, UNIT)
#define BODY Rect(0, UNIT, UNIT, UNIT)
#define TURN Rect(0, 2 * UNIT, UNIT, UNIT)
#define TAIL Rect(UNIT, 3 * UNIT, UNIT, UNIT)

//snake is player if type = 0
Snake* Snake::create(ValueMap vm, GameMap* game_map) {
	Snake *pRet = new(std::nothrow) Snake(vm, game_map);
	if (pRet && pRet->init()) {
		pRet->autorelease();
		return pRet;
	}
	else {
		delete pRet;
		pRet = nullptr;
		return nullptr;
	}
}

Snake::Snake(ValueMap vm, GameMap* game_map) {
	log("start create a snake");
	if (vm.empty()) {
		log("create snake failed!");
	}
	this->game_map = game_map;
	position = game_map->to_tile_map_pos(Vec2(vm["x"].asFloat(), vm["y"].asFloat()));
	if (vm.count("direction") == 0) {
		log("direction of snake has not define");
		vm["direction"] = Value(DIRECTION::UP);
	}
	current_dir = (DIRECTION)vm["direction"].asInt();
	if (vm.count("image") == 0) {
		log("image of snake has not define");
		vm["image"] = Value("snake0.png");
	}
	this->image = vm["image"].asString();
	this->setName(vm["name"].asString());
	log("(%d, %d), dir = %d, image = %s", position.first, position.second, current_dir, vm["image"].asString().c_str());
}

bool Snake::init() {
	if (!SpriteBatchNode::initWithFile(this->image)) {
		return false;
	}
	log("snake init");
	is_died = false;
	snake_nodes = new queue<Sprite*>();
	turn_list = new queue<DIRECTION>();
	length = 2;
	score = 0;
	hunger = 0;
	target = pii(-1, -1);
	new_head();
	this->setAnchorPoint(Vec2::ZERO);
	this->setPosition(Vec2::ZERO + Vec2(UNIT, UNIT) / 2);
	game_map->addChild(this);
	return true;
}

Snake::~Snake() {
	delete turn_list;
	delete snake_nodes;
	log("deleted a snake");
}

bool Snake::go_ahead() {
	if (is_died) {
		return false;
	}
	int s = length - snake_nodes->size();
	if (s == 0) {
		new_tail();
	}
	else if (s > 0) {
		//snake_nodes->back()->setColor(Color3B::GRAY);
	}
	else if (s < 0) {
		new_tail();
		new_tail();
	}
	new_head();
	hunger++;
	return true;
}

bool Snake::new_tail() {
	if (snake_nodes->empty()) {
		return false;
	}
	auto tail = snake_nodes->front();
	if (tail->getName() == "food") {
		tail->setName("tail");
		tail->setColor(Color3B::WHITE);
		length++;
		return false;
	}
	auto pos = game_map->to_tile_map_pos(tail->getPosition());
	game_map->get_snake_map()[pos.first][pos.second] = NULL;
	this->removeChild(tail, true);
	snake_nodes->pop();
	if (!snake_nodes->empty()) {
		snake_nodes->front()->setSpriteFrame(SpriteFrame::create(this->image, TAIL));
	}
	return true;
}

bool Snake::new_head() {
	if (!snake_nodes->empty()) {
		auto next_dir = current_dir;
		if (!turn_list->empty()) {
			next_dir = turn_list->front();
			turn_list->pop();
		}
		Rect rect = TAIL;
		bool flipped_x = false;
		if (snake_nodes->size() >= 2) {
			switch (next_dir - current_dir) {
			case -1:
			case 3:
				flipped_x = true;
			case 1:
			case -3:
				rect = TURN;
				break;
			case 0:
				rect = BODY;
				break;
			}
		}
		current_dir = next_dir;
		snake_nodes->back()->setSpriteFrame(SpriteFrame::create(this->image, rect));
		snake_nodes->back()->setFlippedX(flipped_x);
		snake_nodes->back()->setRotation(90 * current_dir);
		position = game_map->get_next_position(position, current_dir);
	}
	auto head = Sprite::create(this->image, HEAD);
	head->setTag(game_map->get_time_stamp());
	head->setPosition(game_map->to_cocos_pos(position));
	//log("%s arrived (%d, %d)", this->getName().c_str(), position.first, position.second);
	head->setRotation(90 * current_dir);
	snake_nodes->push(head);
	if (!game_map->get_snake_map()[position.first][position.second]) {
		game_map->get_snake_map()[position.first][position.second] = head;
	}
	this->addChild(head);
	return true;
}

bool Snake::check() {
	if (is_died) {
		return false;
	}
	auto wall = game_map->getLayer("wall");
	if (wall != NULL && wall->getTileGIDAt(Vec2(position.first, position.second)) > 0) {
		log("%s p wall", this->getName().c_str());
		go_die();
		return true;
	}
	auto sp = game_map->get_snake_map()[position.first][position.second];
	if (sp && sp != this->snake_nodes->back()) {
		auto snk = (Snake*)sp->getParent();
		if (snk == NULL) {
			log("%s p NULL", this->getName().c_str());
		}
		else {
			log("%s p %s", this->getName().c_str(), snk->getName().c_str());
		}
		go_die();
		return true;
	}
	auto food = game_map->getLayer("food");
	if (food != NULL && food->getTileGIDAt(Vec2(position.first, position.second)) > 0) {
		auto gid = food->getTileGIDAt(Vec2(position.first, position.second));
		this->eat(gid);
		food->setTileGID(0, Vec2(position.first, position.second));
		schedule([this, gid, food](float dt) {
			/*auto points = MyGame::get_game_node()->get_accessible_points(position);
			if (points.empty()) {
				return;
			}
			auto pos = points[random(0, (int)points.size() - 1)];
			food->setTileGID(gid, Vec2(pos.first, pos.second));*/
			auto pos = game_map->get_random_empty_point();
			if (pos.first >= 0) {
				food->setTileGID(gid, Vec2(pos.first, pos.second));
			}
		}, 0, 0, 0, "new food");
	}
	return true;
}

bool Snake::eat(int gid) {
	//log("%s eat", this->getName().c_str());
	hunger = 0;
	target = pii(-1, -1);
	//score += MyGame::get_game_node()->get_speed();
	score += 100;
	snake_nodes->back()->setName("food");
	snake_nodes->back()->setColor(Color3B::GRAY);
	return false;
}

bool Snake::turn(DIRECTION dir) {
	if (turn_list->size() >= 2) {
		return false;
	}
	auto last_turn = current_dir;
	if (!turn_list->empty()) {
		last_turn = turn_list->back();
	}
	if (abs(dir - last_turn) == 1 || abs(dir - last_turn) == 3) {
		turn_list->push(dir);
		return true;
	}
	return false;
}

bool Snake::go_die() {
	is_died = true;
	auto game = (MyGame*)getUserObject();
	game->game_over();
	/*if (snake_nodes->size() > 1) {
		schedule([this](float dt) {
			new_tail();
		}, 1 / 30.0f, snake_nodes->size() - 1, 0, "go die");
	}*/
	/*while (!snake_nodes->empty()) {
		new_tail();
	}*/
	return false;
}

int Snake::get_tail_time_stamp() {
	if (snake_nodes->empty()) {
		return 0;
	}
	return snake_nodes->front()->getTag() -food;
}

bool Snake::act() {
	auto begin_time = clock();
	if (!turn_list->empty()) {
		return false;
	}
	auto width = float_to_int(game_map->getMapSize().width);
	auto height = float_to_int(game_map->getMapSize().height);
	if (get_length() < (width - 2) * (height - 2) * 0.6 || hunger > width * height) {
		bool safe = true;
		if (hunger > width * height * 3) {
			safe = false;
		}
		if (0 && target.first >= 0) {
			auto dir = game_map->get_target_shortest_path_dir(position, current_dir, target, safe);
			if (dir >= 0) {
				turn_list->push((DIRECTION)dir);
				log("act 1, dir = %d, delay = %d", dir, clock() - begin_time);
				return true;
			}
		}
		else {
			auto targets = game_map->get_foods();
			for (auto t : targets) {
				auto dir = game_map->get_target_shortest_path_dir(position, current_dir, t, safe);
				if (dir >= 0) {
					target = t;
					turn_list->push((DIRECTION)dir);
					//log("act 2, dir = %d, delay = %d", dir, clock() - begin_time);
					return true;
				}
			}
		}
	}
	int lenght_step_min;
	auto t = game_map->get_accessible_last_snake_node(position, current_dir, lenght_step_min);
	auto dir = game_map->get_target_longest_path_dir(position, current_dir, t);
	if (dir >= 0) {
		//log("go to (%d, %d)", t.first, t.second);
		turn_list->push((DIRECTION)dir);
		//log("act 3, dir = %d, delay = %d", dir, clock() - begin_time);
		return true;
	}
	log("!!!!!!!!!!!!!!!failed!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
	//go_die();
	dir = random(0, 3);
	this->turn((DIRECTION)dir);
	return false;
}