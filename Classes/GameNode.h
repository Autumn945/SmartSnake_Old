#pragma once
#include "SystemHeader.h"

class Snake;

USING_NS_CC;
using namespace std;
class GameNode : public TMXTiledMap {
public:
	DEFINE_VAR_GET(vector<Snake*>*, player);
	DEFINE_VAR_GET(vector<Snake*>*, AI);
	DEFINE_VAR_GET(set<Sprite*>**, snake_map);
	DEFINE_VAR_GET(mp_pii_pii*, hole_map);
	static int game_width;
	static int game_height;
	~GameNode();
	static GameNode* createWithTMXFile(string file_name);
	virtual bool initWithTMXFile(string);
	virtual void update(float dt);
	static pii to_tile_map_pos(Vec2 pos);
	static Vec2 to_cocos_pos(pii pos);
	vector<pii> get_accessible_points(pii, int);
};

