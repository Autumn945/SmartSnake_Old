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
	menu_back_to_main_menu->setAnchorPoint(Vec2(0, 0));
	menu_back_to_main_menu->setPosition(MyUtility::origin.x, MyUtility::origin.y);
	auto menu = Menu::create(menu_back_to_main_menu, NULL);
	menu->setAnchorPoint(Vec2::ZERO);
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu);
	log("add menu finished");

	game = GameNode::create();
	this->addChild(game);

	control = Node::create();
	auto tmp = MyUtility::visible_size.width - game->getContentSize().width;
	control->setContentSize(Size(tmp, tmp));
	control->setAnchorPoint(Vec2(1, 0));
	control->setPosition(MyUtility::origin.x + MyUtility::visible_size.width, MyUtility::origin.y);
	this->addChild(control);
	log("add control");

	turn_left = Sprite::create(STRING("file_turn_left_control"));
	turn_left->setAnchorPoint(Vec2(0, 1));
	turn_left->setPosition(Vec2(0, control->getContentSize().height));

	turn_right = Sprite::create(STRING("file_turn_left_control"));
	turn_right->setFlippedX(true);
	turn_right->setAnchorPoint(Vec2(1, 1));
	turn_right->setPosition((Vec2)control->getContentSize());

	turn_right_back = Sprite::create(STRING("file_turn_right_back_control"));
	turn_right_back->setAnchorPoint(Vec2(1, 0));
	turn_right_back->setPosition(Vec2(control->getContentSize().width, 0));

	turn_left_back = Sprite::create(STRING("file_turn_right_back_control"));
	turn_left_back->setFlippedX(true);
	turn_left_back->setAnchorPoint(Vec2(0, 0));
	turn_left_back->setPosition(Vec2(0, 0));
	log("sprite init");
	control->addChild(turn_left);
	control->addChild(turn_right);
	control->addChild(turn_left_back);
	control->addChild(turn_right_back);
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority()
	log("control add child");
	return true;
}

bool MyGame::deal_with_event(string event_name) {
	if (MyUtility::deal_with_event(event_name)) {
		return true;
	}
	return false;
}