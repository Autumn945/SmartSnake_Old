#include "MyGameScene.h"

USING_NS_CC;
using namespace std;
string MyGame::map_file_name;


Scene* MyGame::createScene(string file_name) {
	auto scene = Scene::create();
	map_file_name = file_name;
	auto layer = MyGame::create();
	scene->addChild(layer);
	return scene;
}

bool MyGame::init() {
	if (!Layer::init()) {
		return false;
	}
	log("my game init");
	speed = 2;
	step = 0;
	game_map = GameMap::createWithTMXFile(map_file_name);
	CCASSERT(game_map, "gamemap create failed!");
	this->addChild(game_map);
	//player = Snake::create();
	auto snake_obj_group = game_map->getObjectGroup("snake_objs");
	CCASSERT(snake_obj_group, "snake_objs has not defined!");
	auto snake_objs = snake_obj_group->getObjects();
	for (auto _snake : snake_objs) {
		auto snake = _snake.asValueMap();
		auto sp = Snake::create(snake, game_map);
		if (snake["type"].asString() == "player") {
			if (player) {
				log("more than one player, will use the last one");
			}
			player = sp;
		}
		else {
			//AI->push_back(sp);
		}
		game_map->addChild(sp);
	}
	CCASSERT(player, "player has not defined!");
	game_map->addChild(player);
	auto label = Label::createWithSystemFont("score: 0", "Arial", DEFAULT_LABEL_FONT_SIZE);
	label->setAnchorPoint(Vec2(0.5, 1));
	label->setPosition(this->getContentSize().width / 2, 0);
	label->setName("score");
	this->addChild(label);
	scheduleUpdate();

	auto layer = LayerColor::create(Color4B::GRAY, game_map->getContentSize().width, game_map->getContentSize().height);
	if (layer->isIgnoreAnchorPointForPosition()) {
		layer->ignoreAnchorPointForPosition(false);
	}
	layer->setAnchorPoint(game_map->getAnchorPoint());
	layer->setPosition(game_map->getPosition());
	this->addChild(layer, -1);

	auto menu_back = MenuItemFont::create(get_UTF8_string("back"), [this](Ref *sender) {
		Director::getInstance()->popScene();
		/*auto next_scene = MainMenu::createScene();
		auto Transition_scene = TransitionCrossFade::create(SCENE_TURN_TRANSITION_TIME, next_scene);
		Director::getInstance()->replaceScene(Transition_scene);*/
		/*this->removeChild(game_map);
		game_map = GameMap::createWithTMXFile("2.tmx");
		CCASSERT(game_map, "gamenode create failed!");
		this->addChild(game_map);*/
	});
	menu_back->setAnchorPoint(Vec2::ZERO);
	auto menu = Menu::create(menu_back, NULL);
	menu->setAnchorPoint(Vec2::ZERO);
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu);

	//add listener
	auto listener_touch = EventListenerTouchOneByOne::create();
	Vec2 *touch_begin = new Vec2();
	listener_touch->onTouchBegan = [touch_begin](Touch *t, Event *e) {
		//log("touch began on (%.0f, %.0f)", t->getLocation().x, t->getLocation().y);
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
	listener_touch->onTouchEnded = [this, set_dir](Touch *t, Event *e) {
		if (Rect(Vec2::ZERO, game_map->getContentSize()).containsPoint(game_map->convertToNodeSpace(t->getStartLocation()))) {
			auto v = t->getLocation() - t->getStartLocation();
			//log("touch ended, (%.0f, %.0f) --> (%.0f, %.0f)", t->getLocation().x, t->getLocation().y, t->getStartLocation().x, t->getStartLocation().y);
			if (v.isZero()) {
				v = t->getLocation() - (origin + visible_size / 2);
				DIRECTION dir = set_dir(v);
				//log("(%.0f, %.0f), dir = %d", t->getLocation().x, t->getLocation().y, dir);
				player->turn(dir);
			}
		}
		return true;
	};
	listener_touch->onTouchMoved = [this, set_dir, touch_begin](Touch *t, Event *e) {
		//log("touch moved!!");
		auto pos = t->getLocation();
		auto start_pre = t->getPreviousLocation() - *touch_begin;
		auto pre_now = pos - t->getPreviousLocation();
		if (!start_pre.isZero() && abs(Vec2::angle(start_pre, pre_now)) > acos(-1.0f) / 4) {
			*touch_begin = pos;
		}
		if (pos.distance(*touch_begin) > touch_move_len) {
			//log("distence = %f", pos.distance(*touch_begin));
			DIRECTION dir = set_dir(pos - *touch_begin);
			player->turn(dir);
			*touch_begin = pos;
		}
	};
	auto listener_key = EventListenerKeyboard::create();
	listener_key->onKeyPressed = [this](EventKeyboard::KeyCode key, Event *e) {
		DIRECTION dir;
		switch (key) {
		case EventKeyboard::KeyCode::KEY_UP_ARROW:
			dir = DIRECTION::UP;
			break;
		case EventKeyboard::KeyCode::KEY_DOWN_ARROW:
			dir = DIRECTION::DOWN;
			break;
		case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
			dir = DIRECTION::LEFT;
			break;
		case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
			dir = DIRECTION::RIGHT;
			break;
		default:
			return;
		}
		player->turn(dir);
	};
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener_touch, this);
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener_key, this);
	return true;
}

void MyGame::update(float dt) {
	if (speed < 1) {
		speed = 1;
	}
	else if (speed > UNIT / 2) {
		speed = UNIT / 2;
	}
	//speed = 16;
	step += speed;
	if (step >= UNIT) {
		step -= UNIT;
		game_map->add_time_stamp(1);
		// test
		if (!player->get_is_died()) {
			//player->act();
			player->go_ahead();
			auto label = (Label*)this->getChildByName("score");
			label->setString("score: " + Value(player->get_score()).asString());
		}
		// test
		if (!player->get_is_died()) {
			player->check();
		}
	}
}