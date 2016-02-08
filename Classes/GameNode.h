#pragma once
#include "SystemHeader.h"

class Snake;

USING_NS_CC;
using namespace std;
class GameNode : public Node {
public:
	~GameNode();
	virtual bool init();
	CREATE_FUNC(GameNode);
	//CC_SYNTHESIZE_READONLY(static GameNode, instance, _instance);
	DEFINE_VAR_GET_SET(vector<Snake*>*, player);
	DEFINE_VAR_GET_SET(vector<Snake*>*, AI);
	DEFINE_VAR_GET(static TMXTiledMap*, tile_map);
	DEFINE_VAR_GET(static set<Sprite*>**, snake_map);
	DEFINE_VAR_GET(static mp_pii_pii*, hole_map);
	static pii to_tile_map_pos(Vec2 pos) {
		pos = pos / UNIT;
		return pii(float_to_int(pos.x), tile_map->getMapSize().height - float_to_int(pos.y) - 1);
	}
	static Vec2 to_cocos_pos(pii pos) {
		pos.second = tile_map->getMapSize().height - pos.second - 1;
		return Vec2(pos.first, pos.second) * UNIT;
	}
};

