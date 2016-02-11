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
	is_dying = is_died = false;
	snake_nodes = new queue<Sprite*>();
	turn_list = new queue<DIRECTION>();
	food = 1;
	speed = 4;
	step = 0;
	time_stamp = 0;
	score = 0;
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
	if (snake_nodes->empty()) {
		new_head();
	}
	if (speed < 1) {
		speed = 1;
	}
	else if (speed > UNIT) {
		speed;
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
		position.first += GameNode::game_width + dir_vector[current_dir].first;
		position.second += GameNode::game_height + dir_vector[current_dir].second;
		position.first %= GameNode::game_width;
		position.second %= GameNode::game_height;
	}
	if (MyGame::get_game_node()->get_hole_map()->count(position) > 0) {
		position = MyGame::get_game_node()->get_hole_map()->at(position);
	}
	auto head = Sprite::create(this->image, HEAD);
	head->setTag(time_stamp++);
	head->setPosition(MyGame::get_game_node()->to_cocos_pos(position));
	//log("%s arrived (%d, %d)", this->getName().c_str(), position.first, position.second);
	head->setRotation(90 * current_dir);
	snake_nodes->push(head);
	MyGame::get_game_node()->get_snake_map()[position.first][position.second].insert(head);
	this->addChild(head);
	return true;
}

bool Snake::check() {
	if (is_dying || is_died) {
		return false;
	}
	auto wall = MyGame::get_game_node()->getLayer("wall");
	if (wall != NULL && wall->getTileGIDAt(Vec2(position.first, position.second)) > 0) {
		log("%s p wall", this->getName().c_str());
		is_dying = true;
		return true;
	}
	auto mp = MyGame::get_game_node()->get_snake_map();
	for (auto sp : mp[position.first][position.second]) {
		auto snk = (Snake*)sp->getParent();
		if (sp == this->snake_nodes->back()) {
			continue;
		}
		log("%s p %s", this->getName().c_str(), snk->getName().c_str());
		is_dying = true;
		return true;
	}
	auto food = MyGame::get_game_node()->getLayer("food");
	if (food != NULL && food->getTileGIDAt(Vec2(position.first, position.second)) > 0) {
		log("%s eat", this->getName().c_str());
		score += speed;
		this->add_food(1);
		auto gid = food->getTileGIDAt(Vec2(position.first, position.second));
		food->setTileGID(0, Vec2(position.first, position.second));
		schedule([this, gid, food](float dt) {
			auto points = MyGame::get_game_node()->get_accessible_points(position, this->get_current_dir());
			if (points.empty()) {
				return;
			}
			auto pos = points[random(0, (int)points.size() - 1)];
			food->setTileGID(gid, Vec2(pos.first, pos.second));
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

int Snake::get_tail_time_stamp() {
	if (snake_nodes->empty()) {
		return 0;
	}
	return snake_nodes->front()->getTag();
}
