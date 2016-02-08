#include "MyGameScene.h"

USING_NS_CC;
using namespace std;

Scene* MyGame::createScene() {
	auto scene = Scene::create();
	auto layer = MyGame::create();
	if (layer == NULL) {
		log("layer of MyGame creating failed!");
	}
	else {
		scene->addChild(layer);
	}
	return scene;
}

bool MyGame::init() {
	if (!Layer::init()) {
		return false;
	}
	log("my game init");
	//add menu back
	auto menu_back = MenuItemFont::create(get_UTF8_string("back"), [](Ref *sender) {
		auto next_scene = MainMenu::createScene();
		if (next_scene == NULL) {
			log("scene of MainMenu creating failed!");
			return;
		}
		auto Transition_scene = TransitionCrossFade::create(SCENE_TURN_TRANSITION_TIME, next_scene);
		Director::getInstance()->replaceScene(Transition_scene);
	});
	menu_back->setAnchorPoint(Vec2(0, 0));
	menu_back->setPosition(origin.x, origin.y);
	auto menu = Menu::create(menu_back, NULL);
	menu->setAnchorPoint(Vec2::ZERO);
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu);

	//debug
	auto top_label = Label::createWithSystemFont("position", "Arial", TOP_LABEL_FONT_SIZE);
	top_label->setAnchorPoint(Vec2(0.5, 1));
	top_label->setPosition(origin.x + visible_size.width / 2
		, origin.y + visible_size.height);
	this->addChild(top_label, 100);

	game = GameNode::create();
	if (game == NULL) {
		log("node fo game create failed!");
		return false;
	}
	this->addChild(game);

	auto layer = LayerColor::create(Color4B::GRAY, game->getContentSize().width, game->getContentSize().height);
	this->addChild(layer, -1);

	control = Sprite::create("control.png");
	if (control == NULL) {
		log("control.png has not found!");
		return false;
	}
	auto control_pressed = Sprite::create("control_pressed.png");
	if (control_pressed == NULL) {
		log("control_pressed.png has not found!");
		return false;
	}
	auto control_pressed_center = Sprite::create("control_pressed_center.png");
	if (control_pressed_center == NULL) {
		log("control_pressed_center.png has not found!");
		return false;
	}
	control->addChild(control_pressed);
	control->addChild(control_pressed_center);
	auto control_width = visible_size.width - game->getContentSize().width;
	auto scale = control_width / (control->getContentSize().width * sqrt(2.0));
	control->setRotation(-45);
	control->setScale(scale);
	//control->setContentSize(Size(tmp, tmp));
	control->setPosition(origin.x + visible_size.width - control_width / 2
		, origin.y + control_width / 2);
	control_pressed->setPosition(control->getContentSize() / 2);
	control_pressed_center->setPosition(control->getContentSize() / 2);
	this->addChild(control);
	control_pressed->setVisible(false);
	control_pressed_center->setVisible(false);


	//add listener
	auto listener_touch = EventListenerTouchOneByOne::create();
	listener_touch->onTouchBegan = [=](Touch *t, Event *e) {
		auto position = t->getLocation();
		DIRECTION dir = (DIRECTION)-1;
		if (Rect(Vec2::ZERO, control->getContentSize()).containsPoint(control->convertToNodeSpace(position))) {
			position = control->convertToNodeSpaceAR(position);
			if (sqrt(position.x * position.x + position.y * position.y) < control->getContentSize().width / 4) {
				dir = (DIRECTION)4;
			}
			else {
				int a = sign(position.x);
				int b = sign(position.y);
				if (a > 0 && b > 0) {
					dir = DIRECTION::UP;
				}
				else if (a > 0 && b < 0) {
					dir = DIRECTION::RIGHT;
				}
				else if (a < 0 && b < 0) {
					dir = DIRECTION::DOWN;
				}
				else if (a < 0 && b > 0) {
					dir = DIRECTION::LEFT;
				}
			}
			if (dir >= 0 && dir < 4) {
				control_pressed->setVisible(true);
				control_pressed->setRotation(dir * 90);
				for (auto snake : *game->get_player()) {
					snake->turn(dir);
				}
			}
			else if (dir == 4) {
				control_pressed_center->setVisible(true);
			}
		}
		return true;
	};
	listener_touch->onTouchEnded = [=](Touch *t, Event *e) {
		control_pressed->setVisible(false);
		control_pressed_center->setVisible(false);
		return true;
	};
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener_touch, control);
	return true;
}
