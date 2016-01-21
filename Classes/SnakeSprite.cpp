#include "SnakeSprite.h"

USING_NS_CC;
using namespace std;

bool Snake::init() {
	if (!Sprite::init()) {
		return false;
	}
	return true;
}
