#include "GameNode.h"
#include "stdlib.h"

USING_NS_CC;

int GameNode::game_width;
int GameNode::game_height;

GameNode::~GameNode() {
	delete AI;
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
	player = NULL;
	AI = new(nothrow) vector<Snake*>();

	this->setAnchorPoint(Vec2(0.5, 1));
	this->setPosition(Vec2(origin.x + visible_size.width / 2, origin.y + visible_size.height));
	auto ggggg = [this]() {
		return snake_map;
	};

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
	//init hole_map;
	for (int i = 0; i < game_width; i++) {
		for (int j = 0; j < game_height; j++) {
			hole_map[i][j] = pii(i, j);
		}
	}
	if (hole_obj_group == NULL) {
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
			hole_map[pos.first][pos.second] = to;
		}
	}

	speed = 2;
	step = 0;
	time_stamp = 0;
	auto snake_obj_group = this->getObjectGroup("snake_objs");
	CCASSERT(snake_obj_group, "snake_objs has not defined!");
	auto snake_objs = snake_obj_group->getObjects();
	for (auto _snake : snake_objs) {
		auto snake = _snake.asValueMap();
		auto sp = Snake::create(snake);
		if (snake["type"].asString() == "player") {
			player = sp;
		}
		else {
			AI->push_back(sp);
		}
		this->addChild(sp);
	}
	CCASSERT(player, "player has not defined!");
	auto label = Label::createWithSystemFont("score: 0", "Arial", DEFAULT_LABEL_FONT_SIZE);
	label->setAnchorPoint(Vec2(0.5, 1));
	label->setPosition(this->getContentSize().width / 2, 0);
	label->setName("score");
	this->addChild(label);
	scheduleUpdate();
	return true;
}

void GameNode::update(float dt) {
	if (speed < 1) {
		speed = 1;
	}
	else if (speed > UNIT / 2) {
		speed = UNIT / 2;
	}
	speed = 16;
	step += speed;
	if (step >= UNIT) {
		step -= UNIT;
		time_stamp++;
		for (auto ai : *AI) {
			if (!ai->get_is_died()) {
				if (random(0, 9) == 0) {
					ai->add_food(1);
				}
				ai->act();
				ai->go_ahead();
			}
		}
		// test
		if (!player->get_is_died()) {
			/*if (random(0, 9) == 0) {
				player->add_food(1);
			}*/
			player->act();
			player->go_ahead();
			auto label = (Label*)this->getChildByName("score");
			label->setString("score: " + Value(player->get_score()).asString());
		}
		else {
			auto label = (Label*)this->getChildByName("score");
			if (label != NULL) {
				//label->setPosition(origin + visible_size / 2);
				//label->runAction(ScaleTo::create(0.5f, 5));
				label->setName("score2");
			}
		}
		for (auto ai : *AI) {
			if (!ai->get_is_died()) {
				ai->check();
			}
		}
		// test
		if (!player->get_is_died()) {
			player->check();
		}
	}
}

void dfs_points(bool vis[max_game_width][max_game_height], bool not_empty[max_game_width][max_game_height], vector<pii> &ret, pii now) {
	if (vis[now.first][now.second]) {
		return;
	}
	vis[now.first][now.second] = true;
	if (not_empty[now.first][now.second]) {
		return;
	}
	for (int i = 0; i < 4; i++) {
		auto x = (now.first + dir_vector[i].first + GameNode::game_width) % GameNode::game_width;
		auto y = (now.second + dir_vector[i].second + GameNode::game_height) % GameNode::game_height;
		dfs_points(vis, not_empty, ret, pii(x, y));
	}
	ret.push_back(now);
}

vector<pii> GameNode::get_accessible_points(pii begin) {
	vector<pii> ret = get_all_empty_points();
	bool vis[max_game_width][max_game_height] = { false };
	bool not_empty[max_game_width][max_game_height] = { false };
	for (auto e : ret) {
		not_empty[e.first][e.second] = true;
	}
	for (int i = 0; i < 4; i++) {
		auto x = (begin.first + dir_vector[i].first + game_width) % game_width;
		auto y = (begin.second + dir_vector[i].second + game_height) % game_height;
		dfs_points(vis, not_empty, ret, pii(x, y));
	}
	return ret;
}

vector<pii> GameNode::get_all_empty_points() {
	auto wall = this->getLayer("wall");
	auto mp = this->get_snake_map();
	auto food = this->getLayer("food");
	vector<pii> ret;
	for (int i = 0; i < game_width; i++) {
		for (int j = 0; j < game_height; j++) {
			if ((wall == NULL || wall->getTileGIDAt(Vec2(i, j)) == 0)
				&& mp[i][j].empty()
				&& (food == NULL || food->getTileGIDAt(Vec2(i, j)) == 0)) {
				ret.push_back(pii(i, j));
			}
		}
	}
	return ret;
}

pii GameNode::get_random_empty_point() {
	auto wall = this->getLayer("wall");
	auto mp = this->get_snake_map();
	auto food = this->getLayer("food");
	pii ret;
	int r = game_width * game_height / 10;
	do {
		r--;
		if (r < 0) {
			break;
		}
		ret = pii(random(0, game_width - 1),  random(0, game_height - 1));
	} while ((wall != NULL && wall->getTileGIDAt(Vec2(ret.first, ret.second)) > 0)
		|| (!mp[ret.first][ret.second].empty())
		|| (food != NULL && food->getTileGIDAt(Vec2(ret.first, ret.second)) > 0));
	if (r < 0) {
		auto empty_points = get_all_empty_points();
		if (empty_points.empty()) {
			return pii(-1, -1);
		}
		ret = empty_points[random(0, (int)empty_points.size() - 1)];
	}
	return ret;
}

pii GameNode::get_next_position(pii now, DIRECTION dir) {
	auto x = (now.first + dir_vector[dir].first + game_width) % game_width;
	auto y = (now.second + dir_vector[dir].second + game_height) % game_height; 
	return hole_map[x][y];
}

bool GameNode::is_empty(pii pos, int delay) {
	auto wall = this->getLayer("wall");
	if (wall != NULL && wall->getTileGIDAt(Vec2(pos.first, pos.second)) > 0) {
		return false;
	}
	if (delay > 0) {
		for (auto sp : snake_map[pos.first][pos.second]) {
			auto snake = (Snake*)sp->getParent();
			if (sp->getTag() - snake->get_tail_time_stamp() >= delay) {
				return false;
			}
		}
	}
	else if (!snake_map[pos.first][pos.second].empty()) {
		return false;
	}
	return true;
}


vector<pii> GameNode::get_foods() {
	vector<pii> ret;
	auto game_node = MyGame::get_game_node();
	auto food = game_node->getLayer("food");
	if (food == NULL) {
		return ret;
	}
	for (int i = 0; i < game_width; i++) {
		for (int j = 0; j < game_height; j++) {
			if (food->getTileGIDAt(Vec2(i, j)) > 0) {
				//log("get food at (%d, %d)", i, j);
				ret.push_back(pii(i, j));
			}
		}
	}
	return ret;
	//auto hole_map = MyGame::get_game_node()->get_hole_map();
	//bool vis[max_game_width][max_game_height] = { false };
	//queue<pii> q;
	//q.push(pii(0, 0));
	//while (!q.empty()) {
	//	auto now = q.front();
	//	q.pop();
	//	for (int i = 0; i < 4; i++) {
	//		auto nxt = game_node->get_next_position(now, (DIRECTION)i);
	//		if (vis[nxt.first][nxt.second]) {
	//			continue;
	//		}
	//		vis[nxt.first][nxt.second] = true;
	//		if (food->getTileGIDAt(Vec2(nxt.first, nxt.second)) > 0) {
	//			//log("get food at (%d, %d)", nxt.first, nxt.second);
	//			ret.push_back(nxt);
	//			break;
	//		}
	//		q.push(nxt);
	//	}
	//}
	//return ret;
}

pii GameNode::get_accessible_last_snake_node(pii position, int dir, int &lenght_step_min) {
	pii ret = position;
	lenght_step_min = this->get_time_stamp();
	bool vis[max_game_width][max_game_height] = { false };
	auto wall = this->getLayer("wall");
	auto snake_map = this->get_snake_map();
	queue<pair<pii, int> > q;
	q.push(make_pair(position, 0));
	while (!q.empty()) {
		auto now = q.front().first;
		auto step = q.front().second + 1;
		q.pop();
		for (int i = 0; i < 4; i++) {
			auto nxt = this->get_next_position(now, (DIRECTION)i);
			if (vis[nxt.first][nxt.second]) {
				continue;
			}
			vis[nxt.first][nxt.second] = true;
			if (wall != NULL && wall->getTileGIDAt(Vec2(nxt.first, nxt.second)) > 0) {
				continue;
			}
			if (!snake_map[nxt.first][nxt.second].empty()) {
				int time_stamp_max = -step;
				for (auto sp : snake_map[nxt.first][nxt.second]) {
					auto snake = (Snake*)sp->getParent();
					time_stamp_max = max(time_stamp_max, sp->getTag() - snake->get_tail_time_stamp() - step);
				}
				if ((step > 1 || time_stamp_max < 0) && time_stamp_max < lenght_step_min) {
					lenght_step_min = time_stamp_max;
					ret = nxt;
				}
			}
			else {
				q.push(make_pair(nxt, step));
			}
		}
	}
	if (lenght_step_min >= 0) {
		log("ret = (%d, %d), lenght_step_min = %d", ret.first, ret.second, lenght_step_min);
	}
	return ret;
}

int GameNode::get_target_shortest_path_dir(pii position, int current_dir, pii target, bool safe) {
	if (position == target) {
		return 0;
	}
	// otherwhere if (i, j) has not visited, vis[i][j] = 0, otherwise direction is vis[i][j] - 1;
	int vis[max_game_width][max_game_height] = { 0 };
	pii pre_position[max_game_width][max_game_height];
	queue<pii> q;
	q.push(position);
	vis[position.first][position.second] = current_dir + 1;
	bool is_get_target = false;
	while (!q.empty() && !is_get_target) {
		auto now = q.front();
		q.pop();
		for (int i = 0; i < 4; i++) {
			int pre_dir = vis[now.first][now.second] - 1;
			if (abs(i - pre_dir) == 2) {
				continue;
			}
			auto nxt = this->get_next_position(now, (DIRECTION)i);
			if (vis[nxt.first][nxt.second] > 0) {
				continue;
			}
			vis[nxt.first][nxt.second] = i + 1;
			pre_position[nxt.first][nxt.second] = now;
			if (nxt == target) {
				is_get_target = true;
				break;
			}
			if (!this->is_empty(nxt, abs(nxt.first - position.first) + abs(nxt.second - position.second))) {
				continue;
			}
			q.push(nxt);
		}
	}
	if (is_get_target) {
		//log("position = (%d, %d)", position.first, position.second);
		vector<pii> vt;
		int ret = -1;
		while (target != position) {
			vt.push_back(target);
			//log("target = (%d, %d)", target.first, target.second);
			ret = vis[target.first][target.second] - 1;
			target = pre_position[target.first][target.second];
			if (target == position) {
				if (!safe) {
					return ret;
				}
				break;
			}
		}
		target = vt[0];
		int vs = vt.size();
		vector<Sprite*> sps;
		Snake *snake = player;
		//for (auto sp : snake_map[position.first][position.second]) {
		//	auto snk = (Snake*)sp->getParent();
		//	//if (sp == snake->get_snake_nodes()->back())
		//	{
		//		snake = snk;
		//		break;
		//	}
		//}
		//CCASSERT(snake, "snake is null");
		for (int i = vs - 1; i >= 0; i--) {
			auto sp = Sprite::create();
			sp->setTag(time_stamp + vs - 1 - i);
			snake->addChild(sp);
			snake_map[vt[i].first][vt[i].second].insert(sp);
			sps.push_back(sp);
		}
		int length_step_min;
		auto tmp = get_accessible_last_snake_node(target, vis[target.first][target.second] - 1, length_step_min);
		if (length_step_min >= vs - 1) {
			ret = -1;
		}
		for (int i = vs - 1; i >= 0; i--) {
			auto sp = sps[vs - 1 - i];
			snake_map[vt[i].first][vt[i].second].erase(sp);
			snake->removeChild(sp, true);
		}
		return ret;
	}
	return -1;
}

int GameNode::get_target_longest_path_dir(pii position, int current_dir, pii target) {
	//return get_target_shortest_path_dir(position, current_dir, target);
	int ret = -1;
	int dis = -1;
	auto game_node = MyGame::get_game_node();
	int dis_map[max_game_width][max_game_height] = { 0 };
	queue<pii> q;
	q.push(target);
	while (!q.empty()) {
		auto now = q.front();
		q.pop();
		for (int i = 0; i < 4; i++) {
			auto nxt = get_next_position(now, (DIRECTION)i);
			if (!is_empty(nxt) || dis_map[nxt.first][nxt.second] > 0) {
				continue;
			}
			dis_map[nxt.first][nxt.second] = dis_map[now.first][now.second] + 1;
			q.push(nxt);
		}
	}
	for (int i = 0; i < 4; i++) {
		if (abs(i - current_dir) == 2) {
			continue;
		}
		auto nxt = game_node->get_next_position(position, (DIRECTION)i);
		if (game_node->is_empty(nxt, 1) && get_target_shortest_path_dir(nxt, i, target) >= 0) {
			int tmp = dis_map[nxt.first][nxt.second];
			if (tmp > dis) {
				dis = tmp;
				ret = i;
			}
		}
	}
	if (ret == -1) {
		log("dir = %d", current_dir);
		log("position = (%d, %d)", position.first, position.second);
		log("target = (%d, %d)", target.first, target.second);
		log("ret = %d", ret);
	}
	return ret;
}
