#pragma once
#include "SystemHeader.h"

class Snake;

USING_NS_CC;
using namespace std;
class GameNode : public TMXTiledMap {
public:
	DEFINE_VAR_GET(Snake*, player);
	DEFINE_VAR_GET(vector<Snake*>*, AI);
	//DEFINE_VAR_GET(set<Sprite*>[max_game_width][max_game_height], snake_map);
	protected: set<Sprite*> snake_map[max_game_width][max_game_height];
	public: set<Sprite*> (*get_snake_map())[max_game_height]{
		return snake_map;
	}
	//DEFINE_VAR_GET(pii[max_game_width][max_game_height], hole_map);
	protected: pii hole_map[max_game_width][max_game_height];
	public: pii (*get_hole_map())[max_game_height] {
		return hole_map;
	}
	DEFINE_VAR_GET(int, time_stamp);
	DEFINE_VAR_GET_ADD(int, speed);
	DEFINE_VAR_GET_ADD(int, step);
	static int game_width;
	static int game_height;
	static pii to_tile_map_pos(Vec2 pos);
	static Vec2 to_cocos_pos(pii pos);
	~GameNode();
	static GameNode* createWithTMXFile(string file_name);
	virtual bool initWithTMXFile(string);
	virtual void update(float dt);
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

