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
	log("my game init");

	CREATE_MENU_ITEM_WITH_NAME(menu_back_to_main_menu);
	menu_back_to_main_menu->setAnchorPoint(Vec2(1, 0));
	menu_back_to_main_menu->setPosition(MyUtility::origin.x + MyUtility::visible_size.width, MyUtility::origin.y);
	auto menu = Menu::create(menu_back_to_main_menu, NULL);
	menu->setAnchorPoint(Vec2::ZERO);
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu);
	log("add menu finished");

	auto game = GameNode::create();
	log("create game node finished");
	this->addChild(game);
	log("add child finished");
	return true;
}

bool MyGame::deal_with_event(string event_name) {
	if (MyUtility::deal_with_event(event_name)) {
		return true;
	}
	return false;
}