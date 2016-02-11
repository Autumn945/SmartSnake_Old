#include "MyGameScene.h"

USING_NS_CC;
using namespace std;

GameNode* MyGame::game_node;

Scene* MyGame::createScene() {
	auto scene = Scene::create();
	auto layer = MyGame::create();
	CCASSERT(layer, "layer of MyGame creating failed!");
	scene->addChild(layer);
	return scene;
}

bool MyGame::init() {
	if (!Layer::init()) {
		return false;
	}
	log("my game init");

	//debug
	/*
	auto top_label = Label::createWithSystemFont("position", "Arial", TOP_LABEL_FONT_SIZE);
	top_label->setAnchorPoint(Vec2(0.5, 1));
	top_label->setPosition(origin.x + visible_size.width / 2
		, origin.y + visible_size.height);
	this->addChild(top_label, 100);
	*/
	game_node = GameNode::createWithTMXFile("2.tmx");
	CCASSERT(game_node, "gamenode create failed!");
	this->addChild(game_node);

	auto layer = LayerColor::create(Color4B::GRAY, game_node->getContentSize().width, game_node->getContentSize().height);
	if (layer->isIgnoreAnchorPointForPosition()) {
		layer->ignoreAnchorPointForPosition(false);
	}
	layer->setAnchorPoint(game_node->getAnchorPoint());
	layer->setPosition(game_node->getPosition());
	this->addChild(layer, -1);

	//auto control = Sprite::create("control.png");
	//if (control == NULL) {
	//	log("control.png has not found!");
	//	return false;
	//}
	//auto control_pressed = Sprite::create("control_pressed.png");
	//if (control_pressed == NULL) {
	//	log("control_pressed.png has not found!");
	//	return false;
	//}
	//auto control_pressed_center = Sprite::create("control_pressed_center.png");
	//if (control_pressed_center == NULL) {
	//	log("control_pressed_center.png has not found!");
	//	return false;
	//}
	//control->addChild(control_pressed);
	//control->addChild(control_pressed_center);
	//auto control_width = visible_size.width - game_node->getContentSize().width;
	//auto scale = control_width / (control->getContentSize().width * sqrt(2.0));
	//control->setRotation(-45);
	//control->setScale(scale);
	////control->setContentSize(Size(tmp, tmp));
	//control->setPosition(origin.x + visible_size.width - control_width / 2
	//	, origin.y + control_width / 2);
	//control_pressed->setPosition(control->getContentSize() / 2);
	//control_pressed_center->setPosition(control->getContentSize() / 2);
	//this->addChild(control);
	//control_pressed->setVisible(false);
	//control_pressed_center->setVisible(false);
	//add menu back
	auto menu_back = MenuItemFont::create(get_UTF8_string("back"), [](Ref *sender) {
		auto next_scene = MainMenu::createScene();
		CCASSERT(next_scene, "scene of MainMenu creating failed!");
		auto Transition_scene = TransitionCrossFade::create(SCENE_TURN_TRANSITION_TIME, next_scene);
		Director::getInstance()->replaceScene(Transition_scene);
	});
	menu_back->setAnchorPoint(Vec2(0, 0));
	menu_back->setPosition(origin.x, origin.y);
	auto menu_add_speed = MenuItemFont::create("   +   ", [](Ref *sender) {
		for (auto snake : *game_node->get_player()) {
			snake->add_speed(1);
		}
	});
	auto menu_sub_speed = MenuItemFont::create("   -   ", [](Ref *sender) {
		for (auto snake : *game_node->get_player()) {
			snake->add_speed(-1);
		}

	});
	menu_add_speed->setAnchorPoint(Vec2(1, 0));
	menu_add_speed->setPosition(origin + Vec2(visible_size.width, 0));
	menu_sub_speed->setAnchorPoint(Vec2(1, 0));
	menu_sub_speed->setPosition(origin + Vec2(visible_size.width - menu_add_speed->getContentSize().width, 0));
	auto menu = Menu::create(menu_back, menu_add_speed, menu_sub_speed, NULL);
	menu->setAnchorPoint(Vec2::ZERO);
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu);

	//add listener
	auto listener_touch = EventListenerTouchOneByOne::create();
	Vec2 *touch_begin = new Vec2();
	listener_touch->onTouchBegan = [touch_begin](Touch *t, Event *e) {
		log("touch began on (%.0f, %.0f)", t->getLocation().x, t->getLocation().y);
		auto position = t->getLocation();
		*touch_begin = position;
		return true;
	};
	auto set_dir = [](Vec2 v) {
		if (abs(v.x) > abs(v.y)) {
			if (v.x > 0) {
				return DIRECTION::RIGHT;
			}
			return DIRECTION::LEFT;
		}
		if (v.y > 0) {
			return DIRECTION::UP;
		}
		return DIRECTION::DOWN;
	};
	listener_touch->onTouchEnded = [set_dir](Touch *t, Event *e) {
		if (Rect(Vec2::ZERO, game_node->getContentSize()).containsPoint(game_node->convertToNodeSpace(t->getStartLocation()))) {
			auto v = t->getLocation() - t->getStartLocation();
			log("touch ended, (%.0f, %.0f) --> (%.0f, %.0f)", t->getLocation().x, t->getLocation().y, t->getStartLocation().x, t->getStartLocation().y);
			if (v.isZero()) {
				v = t->getLocation() - (origin + visible_size / 2);
				DIRECTION dir = set_dir(v);
				log("(%.0f, %.0f), dir = %d", t->getLocation().x, t->getLocation().y, dir);
				for (auto snake : *game_node->get_player()) {
					snake->turn(dir);
				}
			}
		}
		return true;
	};
	listener_touch->onTouchMoved = [set_dir, touch_begin](Touch *t, Event *e) {
		log("touch moved!!");
		auto pos = t->getLocation();
		auto start_pre = t->getPreviousLocation() - *touch_begin;
		auto pre_now = pos - t->getPreviousLocation();
		if (!start_pre.isZero() && abs(Vec2::angle(start_pre, pre_now)) > acos(-1.0f) / 4) {
			*touch_begin = pos;
		}
		if (pos.distance(*touch_begin) > touch_move_len) {
			log("distence = %f", pos.distance(*touch_begin));
			DIRECTION dir = set_dir(pos - *touch_begin);
			for (auto snake : *game_node->get_player()) {
				snake->turn(dir);
			}
			*touch_begin = pos;
		}
	};
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener_touch, this);
	return true;
}
