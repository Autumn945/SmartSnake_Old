#pragma once
#include "SystemHeader.h"


USING_NS_CC;
using namespace std;
class Snake : public Sprite {
public:
	virtual bool init();
	CREATE_FUNC(Snake);

	enum DIRECTION {
		UP, LEFT, DOWN, RIGHT
	};

	CC_SYNTHESIZE_READONLY(DIRECTION, dir, _dir);
	CC_SYNTHESIZE_READONLY(float, speed, _speed);
};


