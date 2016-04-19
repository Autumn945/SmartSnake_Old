#include "GameMap.h"
#include "stdlib.h"
#include "Snake.h"

USING_NS_CC;

GameMap::~GameMap() {
	log("deleted a GameMap");
}

pii GameMap::to_tile_map_pos(Vec2 pos) {
	pos = pos / UNIT;
	return pii(float_to_int(pos.x), this->getMapSize().height - float_to_int(pos.y) - 1);
}
Vec2 GameMap::to_cocos_pos(pii pos) {
	pos.second = this->getMapSize().height - pos.second - 1;
	return Vec2(pos.first, pos.second) * UNIT;
}

GameMap * GameMap::createWithTMXFile(string file_name) {
	GameMap *pRet = new(std::nothrow) GameMap();
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

bool GameMap::initWithTMXFile(string file_name) {
	if (!TMXTiledMap::initWithTMXFile(file_name)) {
		log("initWithTMXFile %s failed!", file_name.c_str());
		return false;
	}
	log("GameMap init");

	this->setAnchorPoint(Vec2(0.5, 1));
	this->setPosition(Vec2(origin.x + visible_size.width / 2, origin.y + visible_size.height));

	if (this->getLayer("wall") == NULL) {
		log("layer wall of tile_map has not found!");
	}
	if (this->getLayer("grass") == NULL) {
		log("layer grass of tile_map has not found!");
	}
	else {
		this->getLayer("grass")->setLocalZOrder(1);
	}
	auto wall = this->getLayer("wall");
	empty_n = this->getMapSize().width * this->getMapSize().height;
	if (wall) {
		for (int i = 0; i < this->getMapSize().width; i++) {
			for (int j = 0; j < this->getMapSize().height; j++) {
				if (wall->getTileGIDAt(Vec2(i, j)) > 0) {
					empty_n--;
				}
			}
		}
	}
	return true;
}

vector<pii> GameMap::get_accessible_points(pii begin) {
	vector<pii> ret = get_all_empty_points();
	bool vis[max_game_width][max_game_height] = { false };
	bool not_empty[max_game_width][max_game_height] = { false };
	for (auto e : ret) {
		not_empty[e.first][e.second] = true;
	}
	queue<pii> q;
	q.push(begin);
	while (!q.empty()) {
		auto now = q.front();
		q.pop();
		for (int i = 0; i < 4; i++) {
			auto nxt = get_next_position(now, (DIRECTION)i);
			if (not_empty[nxt.first][nxt.second]) {
				q.push(nxt);
				ret.push_back(nxt);
			}
		}
	}
	return ret;
}

vector<pii> GameMap::get_all_empty_points() {
	auto wall = this->getLayer("wall");
	auto mp = this->get_snake_map();
	auto food = this->getLayer("food");
	vector<pii> ret;
	auto width = float_to_int(this->getMapSize().width);
	auto height = float_to_int(this->getMapSize().height);
	for (int i = 0; i < width; i++) {
		for (int j = 0; j < height; j++) {
			if ((wall == NULL || wall->getTileGIDAt(Vec2(i, j)) == 0)
				&& mp[i][j] == NULL
				&& (food == NULL || food->getTileGIDAt(Vec2(i, j)) == 0)) {
				ret.push_back(pii(i, j));
			}
		}
	}
	return ret;
}

pii GameMap::get_random_empty_point() {
	auto wall = this->getLayer("wall");
	auto mp = this->get_snake_map();
	auto food = this->getLayer("food");
	pii ret;
	auto width = float_to_int(this->getMapSize().width);
	auto height = float_to_int(this->getMapSize().height);
	int r = width * height / 10;
	do {
		r--;
		if (r < 0) {
			break;
		}
		ret = pii(random(0, width - 1),  random(0, height - 1));
	} while ((wall != NULL && wall->getTileGIDAt(Vec2(ret.first, ret.second)) > 0)
		|| (mp[ret.first][ret.second])
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

pii GameMap::get_next_position(pii now, int dir) {
	auto width = float_to_int(this->getMapSize().width);
	auto height = float_to_int(this->getMapSize().height);
	auto x = (now.first + dir_vector[dir].first + width) % width;
	auto y = (now.second + dir_vector[dir].second + height) % height; 
	return pii(x, y);
}

bool GameMap::is_empty(pii pos, int delay) {
	if (pos.first < 0 || pos.first >= max_game_width || pos.second < 0 || pos.second >= max_game_height) {
		return false;
	}
	auto wall = this->getLayer("wall");
	if (wall != NULL && wall->getTileGIDAt(Vec2(pos.first, pos.second)) > 0) {
		return false;
	}
	auto sp = snake_map[pos.first][pos.second];
	if (sp) {
		auto snake = (Snake*)sp->getParent();
		int length = sp->getTag() - snake->get_tail_time_stamp() + 1;
		int steps = length * Snake::step_length - snake->get_step();
		auto time_snake = (steps - 1) / snake->get_speed() + 1;
		if (steps <= 0) {
			time_snake = 0;
		}
		if (time_snake > delay) {
			return false;
		}
	}
	return true;
}


vector<pii> GameMap::get_foods() {
	vector<pii> ret;
	auto food = this->getLayer("food");
	if (food == NULL) {
		return ret;
	}
	auto width = float_to_int(this->getMapSize().width);
	auto height = float_to_int(this->getMapSize().height);
	for (int i = 0; i < width; i++) {
		for (int j = 0; j < height; j++) {
			if (food->getTileGIDAt(Vec2(i, j)) > 0) {
				//log("get food at (%d, %d)", i, j);
				ret.push_back(pii(i, j));
			}
		}
	}
	return ret;
}
vector<pii> GameMap::get_foods(Snake* snake) {
	vector<pii> ret;
	auto food = this->getLayer("food");
	if (!food) {
		return ret;
	}
	int vis[max_game_width][max_game_height] = { 0 };
	queue<pair<pii, int> > q;
	q.push(make_pair(snake->get_position(), 0));
	vis[snake->get_position().first][snake->get_position().second] = snake->get_current_dir() + 1;
	while (!q.empty()) {
		auto now = q.front().first;
		auto step = q.front().second + 1;
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
			int steps = step * Snake::step_length - snake->get_step();
			int time_s = (steps - 1) / snake->get_speed() + 1;
			if (steps <= 0) {
				time_s = 0;
			}
			if (food->getTileGIDAt(Vec2(nxt.first, nxt.second)) > 0) {
				ret.push_back(nxt);
			}
			if (this->is_empty(nxt, time_s)) {
				q.push(make_pair(nxt, step));
			}
		}
	}
	return ret;
}

//void GameMap::dfs_for_last_snake_node(pii now, int dir
//	, int vis[max_game_width][max_game_height]
//	, pii pre_position[max_game_width][max_game_height]
//	, Sprite* snake_map[max_game_width][max_game_height]
//	, Snake* snake
//	, int dir_f[4]
//	, int step
//	, int &lenght_step_min
//	, pii &target) {
//	if (vis[now.first][now.second] > 0) {
//		return;
//	}
//	vis[now.first][now.second] = dir + 1;
//	for (int i = 0; i < 4; i++) {
//		if (abs(dir_f[i] - dir) == 2) {
//			continue;
//		}
//		auto nxt = this->get_next_position(now, dir_f[i]);
//		if (vis[nxt.first][nxt.second] > 0) {
//			continue;
//		}
//		pre_position[nxt.first][nxt.second] = now;
//		vis[nxt.first][nxt.second] = dir_f[i] + 1;
//		auto sp = snake_map[nxt.first][nxt.second];
//		if (sp && sp->getParent() == snake) {
//			int lenght_step = sp->getTag() - snake->get_tail_time_stamp() - step;
//			if (lenght_step < lenght_step_min) {
//				lenght_step_min = lenght_step;
//				target = nxt;
//			}
//		}
//		auto steps = step * Snake::step_length - snake->get_step();
//		auto time_snake = (steps - 1) / snake->get_step() + 1;
//		if (steps <= 0) {
//			time_snake = 0;
//		}
//		if (is_empty(nxt, time_snake)) {
//			//q.push(make_pair(nxt, step));
//		}
//	}
//}

int GameMap::get_accessible_last_snake_node_dir(pii position, int dir, Snake* snake, int &start_step) {
	//int dir_f[4] = { 0, 1, 2, 3 };
	int ret = -1;
	int lenght_step_min = 0x3fffff;
	auto snake_map = this->get_snake_map();
	pii target = pii(-1, -1);
	for (int dir_t = 0; dir_t < 8; dir_t++) {
		pii target_tmp = position;
		int ret_tmp = -1;
		int lenght_step_min_tmp = 0x3fffff;
		// otherwhere if (i, j) has not visited, vis[i][j] = 0, otherwise direction is vis[i][j] - 1;
		int vis[max_game_width][max_game_height] = { 0 };
		struct SnakeNodeData {
			pii pos;
			int step, dir;
			SnakeNodeData(pii _pos, int _step, int _dir) : pos(_pos), step(_step), dir(_dir) {}
		};
		stack<SnakeNodeData> q;
		q.push(SnakeNodeData(position, start_step, dir));
		while (!q.empty()) {
			auto now = q.top().pos;
			auto step = q.top().step + 1;
			//log("step = %d", step);
			auto dir = q.top().dir;
			q.pop();
			if (vis[now.first][now.second] > 0) {
				continue;
			}
			vis[now.first][now.second] = dir + 1;
			auto sp = snake_map[now.first][now.second];
			if (sp && sp->getParent() == snake) {
				int lenght_step = sp->getTag() - snake->get_tail_time_stamp() - step;
				if (lenght_step < lenght_step_min_tmp) {
					lenght_step_min_tmp = lenght_step;
					target_tmp = now;
				}
			}
			auto steps = step * Snake::step_length - snake->get_step();
			auto time_snake = (steps - 1) / snake->get_step() + 1;
			if (steps <= 0) {
				time_snake = 0;
			}
			for (int i = 0; i < 4; i++) {
				int c_dir = (dir_t / 2 + i) & 3;
				if (dir_t & 1) {
					c_dir = (dir_t / 2 - i) & 3;
				}
				if (abs(dir - c_dir) == 2) {
					continue;
				}
				auto nxt = this->get_next_position(now, c_dir);
				if (is_empty(nxt, time_snake)) {
					q.push(SnakeNodeData(nxt, step, c_dir));
				}
			}
		}
		while (target_tmp != position) {
			ret_tmp = vis[target_tmp.first][target_tmp.second] - 1;
			target_tmp = this->get_next_position(target_tmp, (ret_tmp + 2) & 3);
		}
		if (ret_tmp < 0) {
			break;
		}
		if (lenght_step_min_tmp < lenght_step_min) {
			lenght_step_min = lenght_step_min_tmp;
			ret = ret_tmp;
			target = target_tmp;
		}
	}
	start_step = lenght_step_min;
	/*if (lenght_step_min >= 0) {
		log("ret = (%d, %d), lenght_step_min = %d", target.first, target.second, lenght_step_min);
	}*/
	return ret;
}

int GameMap::get_target_shortest_path_dir(pii position, int current_dir, pii target, Snake* snake, bool safe) {
	if (position == target) {
		return 0;
	}
	// otherwhere if (i, j) has not visited, vis[i][j] = 0, otherwise direction is vis[i][j] - 1;
	int vis[max_game_width][max_game_height] = { 0 };
	pii pre_position[max_game_width][max_game_height];
	queue<pair<pii, int> > q;
	q.push(make_pair(position, 0));
	vis[position.first][position.second] = current_dir + 1;
	bool is_get_target = false;
	while (!q.empty() && !is_get_target) {
		auto now = q.front().first;
		auto step = q.front().second + 1;
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
			int steps = step * Snake::step_length - snake->get_step();
			int time_s = (steps - 1) / snake->get_speed() + 1;
			if (steps <= 0) {
				time_s = 0;
			}
			if (!this->is_empty(nxt, time_s)) {
				continue;
			}
			q.push(make_pair(nxt, step));
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
		for (int i = vs - 1; i >= 0; i--) {
			if (snake_map[vt[i].first][vt[i].second] == NULL) {
				auto sp = Sprite::create();
				sp->setName("virtual_snake");
				sp->setTag(snake->get_time_stamp() + vs - i);
				sp->setParent(snake);
				snake_map[vt[i].first][vt[i].second] = sp;
			}
		}
		int length_step_min = vs - 1;
		auto tmp = get_accessible_last_snake_node_dir(target, vis[target.first][target.second] - 1, snake, length_step_min);
		if (length_step_min >= 0) {
			ret = -1;
		}
		for (int i = vs - 1; i >= 0; i--) {
			if (snake_map[vt[i].first][vt[i].second]->getName() == "virtual_snake") {
				//snake->removeChild(snake_map[vt[i].first][vt[i].second], true);
				snake_map[vt[i].first][vt[i].second] = NULL;
			}
		}
		return ret;
	}
	return -1;
}

int GameMap::get_target_longest_path_dir(pii position, int current_dir, pii target, Snake* snake) {
	//return get_target_shortest_path_dir(position, current_dir, target);
	int ret = -1;
	int dis = -1;
	int dis_map[max_game_width][max_game_height] = { 0 };
	queue<pii> q;
	q.push(target);
	while (!q.empty()) {
		auto now = q.front();
		q.pop();
		for (int i = 0; i < 4; i++) {
			auto nxt = get_next_position(now, (DIRECTION)i);
			if (get_snake_map()[nxt.first][nxt.second] && get_snake_map()[target.first][target.second]
				&& get_snake_map()[nxt.first][nxt.second]->getTag() > get_snake_map()[target.first][target.second]->getTag()) {
				continue;
			}
			int steps = abs(nxt.first - position.first) + abs(nxt.second - position.second) * Snake::step_length - snake->get_step();
			int time_s = (steps - 1) / snake->get_speed() + 1;
			if (steps <= 0) {
				time_s = 0;
			}
			if (!is_empty(nxt, time_s) || dis_map[nxt.first][nxt.second] > 0) {
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
		auto nxt = this->get_next_position(position, (DIRECTION)i);
		if (this->is_empty(nxt)
			&& dis_map[nxt.first][nxt.second] > 0) {
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
