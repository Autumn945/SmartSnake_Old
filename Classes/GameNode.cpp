#include "GameNode.h"

bool GameNode::init() {
	if (!Node::init()) {
		return false;
	}
	//create node game
	game_width = INTEGER("game_width");
	game_height = INTEGER("game_height");
	this->setContentSize(Size(UNIT * game_width, UNIT * game_height));
	this->setAnchorPoint(Vec2(0, 1));
	this->setPosition(MyUtility::origin.x, MyUtility::origin.y + MyUtility::visible_size.height);
	mp = new vector<bool>[game_width];
	mp[0] = vector<bool>(game_width, false);
	return false;
}
