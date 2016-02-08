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
	if (vm.count("direction") == 0) {
		vm["direction"] = Value(DIRECTION::UP);
	}
	if (vm.count("image") == 0) {
		vm["image"] = Value("snake0.png");
	}
	position = GameNode::to_tile_map_pos(Vec2(vm["x"].asFloat(), vm["y"].asFloat()));
	current_dir = (DIRECTION)vm["direction"].asInt();
	log("(%d, %d), dir = %d, image = %s", position.first, position.second, current_dir, vm["image"].asString().c_str());
	this->image = vm["image"].asString();
	this->setName(vm["name"].asString());
}

bool Snake::init() {
	if (!Node::init()) {
		return false;
	}
	is_dying = is_died = false;
	snake_nodes = new queue<Sprite*>();
	turn_list = new queue<DIRECTION>();
	food = 1;
	speed = 4;
	step = 0;
	time_stamp = 0;
	new_head();

	this->setAnchorPoint(Vec2::ZERO);
	this->setPosition(Vec2::ZERO + Vec2(UNIT, UNIT) / 2);

	return true;
}

Snake::~Snake() {
	log("deleted a snake");
}

bool Snake::go_ahead() {
	if (is_died) {
		return false;
	}
	if (is_dying) {
		if (snake_nodes->size() >= 2) {
			new_tail();
		}
		else {
			is_died = true;
		}
		return false;
	}
	if (step < UNIT) {
		step += speed;
		return false;
	}
	step -= UNIT;
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
	go_ahead();
	return true;
}

bool Snake::new_tail() {
	if (snake_nodes->empty()) {
		return false;
	}
	auto pos = GameNode::to_tile_map_pos(snake_nodes->front()->getPosition());
	GameNode::get_snake_map()[pos.first][pos.second].erase(snake_nodes->front());
	this->removeChild(snake_nodes->front());
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
		position.first += game_width + dir_vector[current_dir].first;
		position.second += game_height + dir_vector[current_dir].second;
		position.first %= game_width;
		position.second %= game_height;
	}
	if (GameNode::get_hole_map()->count(position) > 0) {
		position = GameNode::get_hole_map()->at(position);
	}
	auto head = Sprite::create(this->image, HEAD);
	head->setTag(time_stamp++);
	head->setPosition(GameNode::to_cocos_pos(position));
	log("%s arrived (%d, %d)", this->getName().c_str(), position.first, position.second);
	head->setRotation(90 * current_dir);
	snake_nodes->push(head);
	GameNode::get_snake_map()[position.first][position.second].insert(head);
	this->addChild(head);
	return true;
}

bool Snake::check() {
	if (is_dying || is_died) {
		return false;
	}
	auto wall = GameNode::get_tile_map()->getLayer("wall");
	if (wall != NULL && wall->getTileGIDAt(Vec2(position.first, position.second)) > 0) {
		log("%s p wall", this->getName().c_str());
		is_dying = true;
		return true;
	}
	auto mp = GameNode::get_snake_map();
	for (auto sp : mp[position.first][position.second]) {
		auto snk = (Snake*)sp->getParent();
		if (sp == this->snake_nodes->back()) {
			continue;
		}
		log("%s p %s", this->getName().c_str(), snk->getName().c_str());
		is_dying = true;
		return true;
	}
	auto food = GameNode::get_tile_map()->getLayer("food");
	if (food != NULL && food->getTileGIDAt(Vec2(position.first, position.second)) > 0) {
		log("%s eat", this->getName().c_str());
		this->add_food(1);
		food->setTileGID(0, Vec2(position.first, position.second));
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

int Snake::get_tail_time_stamp() {
	if (snake_nodes->empty()) {
		return 0;
	}
	return snake_nodes->front()->getTag();
}