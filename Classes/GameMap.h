#pragma once
#include "SystemHeader.h"

class Snake;

USING_NS_CC;
using namespace std;
class GameMap : public TMXTiledMap {
public:
	protected: Sprite* snake_map[max_game_width][max_game_height];
	public: Sprite* (*get_snake_map())[max_game_height]{
		return snake_map;
	}
	protected: pii hole_map[max_game_width][max_game_height];
	public: pii (*get_hole_map())[max_game_height] {
		return hole_map;
	}
	DEFINE_VAR_GET_ADD(int, time_stamp);
	pii to_tile_map_pos(Vec2 pos);
	Vec2 to_cocos_pos(pii pos);
	~GameMap();
	static GameMap* createWithTMXFile(string file_name);
	virtual bool initWithTMXFile(string);
	vector<pii> get_accessible_points(pii);
	vector<pii> get_all_empty_points();
	pii get_random_empty_point();
	pii get_next_position(pii now, DIRECTION dir);
	bool is_empty(pii pos, int delay = 0);

	vector<pii> get_foods();
	pii get_accessible_last_snake_node(pii position, int dir, int &);
	int get_target_shortest_path_dir(pii position, int current_dir, pii target, bool safe = false);
	int get_target_longest_path_dir(pii position, int current_dir, pii target);
};