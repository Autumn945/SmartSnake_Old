#include "MyGameScene.h"

USING_NS_CC;
using namespace std;

Scene* MyGame::createScene() {
	auto scene = Scene::create();
	auto layer = MyGame::create();
	scene->addChild(layer);
	return scene;
}

bool MyGame::init() {
	if (!Layer::init()) {
		return false;
	}
	return true;
}