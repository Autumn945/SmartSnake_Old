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

	//set touch
	this->setTouchEnabled(true);
	this->setTouchMode(Touch::DispatchMode::ONE_BY_ONE);

	//add menu back
	CREATE_MENU_ITEM_WITH_NAME(menu_back_to_main_menu);
	menu_back_to_main_menu->setAnchorPoint(Vec2(0, 0));
	menu_back_to_main_menu->setPosition(origin.x, origin.y);
	auto menu = Menu::create(menu_back_to_main_menu, NULL);
	menu->setAnchorPoint(Vec2::ZERO);
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu);
	log("add menu finished");

	//add mouse listener
	auto listener = EventListenerMouse::create();
	listener->onMouseDown = [this](Event *event) {
		log("mouse down");
		auto e = (EventMouse *)event;
		deal_with_touch(e->getLocationInView());
	};
	listener->onMouseUp = [this](Event *event) {
		//deal_with_event("touch_released");
	};
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);

	//test
	top_label = Label::createWithSystemFont("position", "Arial", TOP_LABEL_FONT_SIZE);
	top_label->setAnchorPoint(Vec2(0.5, 1));
	top_label->setPosition(origin.x + visible_size.width / 2
		, origin.y + visible_size.height);
	this->addChild(top_label, 100);

	game = GameNode::create();
	this->addChild(game);

	control = Node::create();
	auto tmp = visible_size.width - game->getContentSize().width;
	control->setContentSize(Size(tmp, tmp));
	control->setAnchorPoint(Vec2(0.5, 0.5));
	control->setPosition(origin.x + visible_size.width - control->getContentSize().width / 2
		, origin.y + control->getContentSize().height / 2);
	this->addChild(control);
	log("add control");

	turn_left = Sprite::create(get_UTF8_string("file_turn_left_control"));
	turn_left->setAnchorPoint(Vec2(0, 1));
	turn_left->setPosition(Vec2(0, control->getContentSize().height));

	turn_right = Sprite::create(get_UTF8_string("file_turn_left_control"));
	turn_right->setFlippedX(true);
	turn_right->setAnchorPoint(Vec2(1, 1));
	turn_right->setPosition((Vec2)control->getContentSize());

	turn_right_back = Sprite::create(get_UTF8_string("file_turn_right_back_control"));
	turn_right_back->setAnchorPoint(Vec2(1, 0));
	turn_right_back->setPosition(Vec2(control->getContentSize().width, 0));

	turn_left_back = Sprite::create(get_UTF8_string("file_turn_right_back_control"));
	turn_left_back->setFlippedX(true);
	turn_left_back->setAnchorPoint(Vec2(0, 0));
	turn_left_back->setPosition(Vec2(0, 0));
	log("sprite init");
	control->addChild(turn_left);
	control->addChild(turn_right);
	control->addChild(turn_left_back);
	control->addChild(turn_right_back);
	log("control add child");
	pre_dir_v = 0;
	auto tmp1 = game->get_player();
	log("tmp1 get");
	auto tmp2 = tmp1->get_current_dir();
	log("tmp2 = %d", tmp2);
	control->setRotation(game->get_player()->get_current_dir() * 90);
	log("control set r finished");
	scheduleUpdate();
	return true;
}

bool MyGame::deal_with_event(string event_name) {
	top_label->setString(top_label->getString() + event_name);
	if (event_name == "turn_left") {
		game->get_player()->set_next_turn(-1);
		return true;
	}
	if (event_name == "turn_right") {
		game->get_player()->set_next_turn(1);
		return true;
	}
	if (event_name == "turn_left_back") {
		game->get_player()->set_next_turn(-2);
		return true;
	}
	if (event_name == "turn_right_back") {
		game->get_player()->set_next_turn(2);
		return true;
	}
	if (event_name == "touch_released") {
		game->get_player()->set_next_turn(0);
		return true;
	}
	DEAL_WITH_EVENT;
	return false;
}

bool MyGame::onTouchBegan(Touch *touch, Event *event) {
	return deal_with_touch(touch->getLocation());
}

void MyGame::onTouchEnded(Touch *touch, Event *event) {
	//deal_with_event("touch_released");
}

bool MyGame::is_tap(Node *node, Vec2 position) {
	Vec2 position_in_node = node->convertToNodeSpace(position);
	return Rect(Vec2::ZERO, node->getContentSize()).containsPoint(position_in_node);
}

bool MyGame::deal_with_touch(Vec2 position) {
	log("touch begin on (%f, %f)", position.x, position.y);
	top_label->setString(String::createWithFormat("touch begin on (%.0f, %.0f). ", position.x, position.y)->getCString());
	if (is_tap(turn_left, position)) {
		log("turn left!");
		deal_with_event("turn_left");
		return true;
	}
	if (is_tap(turn_right, position)) {
		log("turn_right!");
		deal_with_event("turn_right");
		return true;
	}
	if (is_tap(turn_left_back, position)) {
		log("turn left_back!");
		deal_with_event("turn_left_back");
		return true;
	}
	if (is_tap(turn_right_back, position)) {
		log("turn_right_back!");
		deal_with_event("turn_right_back");
		return true;
	}
	return false;
}

void MyGame::update(float dt) {
	Layer::update(dt);
	if (pre_dir_v != game->get_player()->get_next_turn()) {
		log("update pre dir v");
		control->setRotation(game->get_player()->get_current_dir() * 90);
		turn_right->setSpriteFrame(Sprite::create(get_UTF8_string("file_turn_left_control"))->getSpriteFrame());
		turn_left->setSpriteFrame(Sprite::create(get_UTF8_string("file_turn_left_control"))->getSpriteFrame());
		turn_right_back->setSpriteFrame(Sprite::create(get_UTF8_string("file_turn_right_back_control"))->getSpriteFrame());
		turn_left_back->setSpriteFrame(Sprite::create(get_UTF8_string("file_turn_right_back_control"))->getSpriteFrame());
		pre_dir_v = game->get_player()->get_next_turn();
		switch (game->get_player()->get_next_turn()) {
		case 1:
			//turn_right->initWithFile(get_UTF8_string("file_turn_left_control_s"));
			//turn_right->setFlippedX(true);
			turn_right->setSpriteFrame(Sprite::create(get_UTF8_string("file_turn_left_control_s"))->getSpriteFrame());
			break;
		case -1:
			turn_left->setSpriteFrame(Sprite::create(get_UTF8_string("file_turn_left_control_s"))->getSpriteFrame());
			break;
		case 2:
			turn_right_back->setSpriteFrame(Sprite::create(get_UTF8_string("file_turn_right_back_control_s"))->getSpriteFrame());
			break;
		case -2:
			turn_left_back->setSpriteFrame(Sprite::create(get_UTF8_string("file_turn_right_back_control_s"))->getSpriteFrame());
			//turn_left_back->setFlippedX(true);
			break;
		default:
			break;
		}
	}
}