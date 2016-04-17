#include "SmartSnake.h"
#include "MyGameScene.h"

//snake is player if type = 0
SmartSnake* SmartSnake::create(string name, GameMap* game_map) {
	SmartSnake *pRet = new(std::nothrow) SmartSnake();
	if (pRet && pRet->init(name, game_map)) {
		pRet->autorelease();
		return pRet;
	}
	else {
		delete pRet;
		pRet = nullptr;
		return nullptr;
	}
}

bool SmartSnake::init(string name, GameMap* game_map) {
	log("snake init");
	if (!Snake::init(name, game_map)) {
		return false;
	}
	target = pii(-1, -1);
	return true;
}

void SmartSnake::eat_reward(int gid) {
	target = pii(-1, -1);
}

bool SmartSnake::go_die() {
	is_died = true; 
	auto game = (MyGame*)game_map->getParent();
	game->kill--;
	auto label = (Label*)game->getChildByName("label_kill");
	if (label) {
		if (game->kill > 0) {
			label->setString(" x" + Value(game->kill).asString());
		}
		else {
			label->setString(" ok!");
		}
	}
	/*auto game = (MyGame*)getUserObject();
	if (snake_nodes->size() > 1) {
		schedule([this](float dt) {
			new_tail();
		}, 1 / 30.0f, snake_nodes->size() - 1, 0, "go die");
	}*/
	return true;
	game_map->add_empty_n(snake_nodes->size());
	while (!snake_nodes->empty()) {
		new_tail();
	}
	return true;
}

bool SmartSnake::go_ahead() {
	if (!is_died && type == Snake::SnakeType::t_enemy) {
		act();
	}
	if (!Snake::go_ahead()) {
		return false;
	}
	return false;
}

Snake::SnakeType SmartSnake::get_type() {
	return type;
}

bool SmartSnake::act() {
	auto begin_time = clock();
	if (turn_1 >= 0) {
		return false;
	}
	bool safe = true;
	if (hunger > 100) {
		safe = false;
	}
	if (target.first >= 0) {
		auto food = game_map->getLayer("food");
		if (food->getTileGIDAt(Vec2(target.first, target.second)) == 0) {
			target = pii(-1, -1);
		}
	}
	if (target.first >= 0) {
		auto dir = game_map->get_target_shortest_path_dir(position, current_dir, target, this, safe);
		if (dir >= 0) {
			turn_1 = dir;
			if (!game_map->is_empty(game_map->get_next_position(position, dir))) {
				log("target = (%d, %d), dir = %d", target.first, target.second, dir);
			}
			//log("act 1, dir = %d, delay = %d", dir, clock() - begin_time);
			return true;
		}
	}
	else {
		auto targets = game_map->get_foods();
		for (auto t : targets) {
			auto dir = game_map->get_target_shortest_path_dir(position, current_dir, t, this, safe);
			if (dir >= 0) {
				if (!game_map->is_empty(game_map->get_next_position(position, dir))) {
					log("target = (%d, %d), dir = %d", target.first, target.second, dir);
				}
				target = t;
				turn_1 = dir;
				//log("act 2, dir = %d, delay = %d", dir, clock() - begin_time);
				return true;
			}
		}
	}
	int lenght_step_min;
	auto dir = game_map->get_accessible_last_snake_node_dir(position, current_dir, this, lenght_step_min);
	if (dir >= 0) {
		//log("go to (%d, %d)", t.first, t.second);
		turn_1 = dir;
		//log("act 3, dir = %d, delay = %d", dir, clock() - begin_time);
		return true;
	}
	log("!!!!!!!!!!!!!!!failed!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
	for (int i = 0; i < 4; i++) {
		if (abs(i - current_dir) != 2 && game_map->is_empty(game_map->get_next_position(position, i))) {
			turn_1 = i;
			return true;
		}
	}
	return false;
}
