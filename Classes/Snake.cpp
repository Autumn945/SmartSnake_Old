#include "Snake.h"

#define HEAD Rect(0, 0, UNIT, UNIT)
#define BODY Rect(0, UNIT, UNIT, UNIT)
#define TURN Rect(0, 2 * UNIT, UNIT, UNIT)
#define TAIL Rect(UNIT, 3 * UNIT, UNIT, UNIT)

//snake is player if type = 0
Snake* Snake::create(ValueMap vm) {
	Snake *pRet = new(std::nothrow) Snake(vm);
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

int Snake::get_length() {
	return snake_nodes->size() + food;
}

Snake::Snake(ValueMap vm) {
	log("start create a snake");
	if (vm.empty()) {
		log("create snake failed!");
	}
	position = GameNode::to_tile_map_pos(Vec2(vm["x"].asFloat(), vm["y"].asFloat()));
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
	food = 1;
	score = 0;
	hunger = 0;
	target = pii(-1, -1);
	this->setAnchorPoint(Vec2::ZERO);
	this->setPosition(Vec2::ZERO + Vec2(UNIT, UNIT) / 2);

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
	if (snake_nodes->empty()) {
		new_head();
	}
	if (food == 0) {
		new_tail();
	}
	else if (food > 0) {
		food--;
	}
	else if (food < 0) {
		new_tail();
		new_tail();
		food++;
	}
	new_head();
	hunger++;
	return true;
}

bool Snake::new_tail() {
	if (snake_nodes->empty()) {
		return false;
	}
	auto pos = MyGame::get_game_node()->to_tile_map_pos(snake_nodes->front()->getPosition());
	MyGame::get_game_node()->get_snake_map()[pos.first][pos.second].erase(snake_nodes->front());
	this->removeChild(snake_nodes->front(), true);
	snake_nodes->pop();
	if (!snake_nodes->empty()) {
		snake_nodes->front()->setSpriteFrame(SpriteFrame::create(this->image, TAIL));
	}
	return true;
}

bool Snake::new_head() {
	auto game_node = MyGame::get_game_node();
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
		position = game_node->get_next_position(position, current_dir);
	}
	auto head = Sprite::create(this->image, HEAD);
	head->setTag(game_node->get_time_stamp());
	head->setPosition(game_node->to_cocos_pos(position));
	//log("%s arrived (%d, %d)", this->getName().c_str(), position.first, position.second);
	head->setRotation(90 * current_dir);
	snake_nodes->push(head);
	MyGame::get_game_node()->get_snake_map()[position.first][position.second].insert(head);
	this->addChild(head);
	return true;
}

bool Snake::check() {
	if (is_died) {
		return false;
	}
	auto game_node = MyGame::get_game_node();
	auto wall = game_node->getLayer("wall");
	if (wall != NULL && wall->getTileGIDAt(Vec2(position.first, position.second)) > 0) {
		log("%s p wall", this->getName().c_str());
		go_die();
		return true;
	}
	auto mp = game_node->get_snake_map();
	for (auto sp : mp[position.first][position.second]) {
		auto snk = (Snake*)sp->getParent();
		if (sp == this->snake_nodes->back()) {
			continue;
		}
		log("%s p %s", this->getName().c_str(), snk->getName().c_str());
		go_die();
		return true;
	}
	auto food = game_node->getLayer("food");
	if (food != NULL && food->getTileGIDAt(Vec2(position.first, position.second)) > 0) {
		//log("%s eat", this->getName().c_str());
		hunger = 0;
		target = pii(-1, -1);
		//score += MyGame::get_game_node()->get_speed();
		score++;
		this->add_food(1);
		auto gid = food->getTileGIDAt(Vec2(position.first, position.second));
		food->setTileGID(0, Vec2(position.first, position.second));
		schedule([this, gid, food](float dt) {
			/*auto points = MyGame::get_game_node()->get_accessible_points(position);
			if (points.empty()) {
				return;
			}
			auto pos = points[random(0, (int)points.size() - 1)];
			food->setTileGID(gid, Vec2(pos.first, pos.second));*/
			auto pos = MyGame::get_game_node()->get_random_empty_point();
			if (pos.first >= 0) {
				food->setTileGID(gid, Vec2(pos.first, pos.second));
			}
		}, 0, 0, 0, "new food");
	}
	return true;
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
	auto game_node = MyGame::get_game_node();
	if (get_length() < (GameNode::game_width - 2) * (GameNode::game_height - 2) * 0.7 || hunger > GameNode::game_width * GameNode::game_height) {
		bool safe = true;
		if (hunger > GameNode::game_width * GameNode::game_height * 2) {
			safe = false;
		}
		if (0 && target.first >= 0) {
			auto dir = game_node->get_target_shortest_path_dir(position, current_dir, target, safe);
			if (dir >= 0) {
				turn_list->push((DIRECTION)dir);
				log("act 1, dir = %d, delay = %d", dir, clock() - begin_time);
				return true;
			}
		}
		else {
			auto targets = game_node->get_foods();
			for (auto t : targets) {
				auto dir = game_node->get_target_shortest_path_dir(position, current_dir, t, safe);
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
	auto t = game_node->get_accessible_last_snake_node(position, current_dir, lenght_step_min);
	auto dir = game_node->get_target_longest_path_dir(position, current_dir, t);
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