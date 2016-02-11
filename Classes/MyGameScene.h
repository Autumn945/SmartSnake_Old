#pragma once
#include "SystemHeader.h"

class GameNode;

USING_NS_CC;
using namespace std;
class MyGame : public Layer {
public:
	static Scene* createScene();
	virtual bool init();
	CREATE_FUNC(MyGame);
	DEFINE_VAR_GET(static GameNode*, game_node);

	//test
	Label *top_label;
};

