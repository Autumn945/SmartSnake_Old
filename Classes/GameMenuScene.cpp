#include "GameMenuScene.h"

USING_NS_CC;
using namespace std;

Scene* GameMenu::createScene() {
	auto scene = Scene::create();
	auto layer = GameMenu::create();
	scene->addChild(layer);
	return scene;
}

bool GameMenu::init() {
	if (!Layer::init()) {
		return false;
	}
	world = GameMap::createWithTMXFile("world.tmx");
	CCASSERT(world, "world create failed!");
	this->addChild(world);
	auto game_map_group = world->getObjectGroup("game_map_objs");
	CCASSERT(game_map_group, "game_map_objs has not defined!");
	auto game_map_objs = game_map_group->getObjects();
	for (auto _obj : game_map_objs) {
		auto obj = _obj.asValueMap();
		game_map[world->to_tile_map_pos(Vec2(obj["x"].asFloat(), obj["y"].asFloat()))] = obj["name"].asString();
	}
	auto snake_obj_group = world->getObjectGroup("snake_objs");
	CCASSERT(snake_obj_group, "snake_objs has not defined!");
	auto snake_objs = snake_obj_group->getObjects();
	for (auto _snake : snake_objs) {
		auto snake = _snake.asValueMap();
		if (user_info.count("game_menu_position_x") > 0) {
			auto position = pii(user_info["game_menu_position_x"].asInt(), user_info["game_menu_position_y"].asInt());
			auto pos = world->to_cocos_pos(position);
			snake["x"] = pos.x;
			snake["y"] = pos.y;
			snake["direction"] = user_info["game_menu_direction"];
		}
		player = Snake::create(snake, world);
	}
	CCASSERT(player, "player has not defined!");
	player->set_length(4);
	//player->go_ahead();
	auto layer = LayerColor::create(Color4B::GRAY, world->getContentSize().width, world->getContentSize().height);
	if (layer->isIgnoreAnchorPointForPosition()) {
		layer->ignoreAnchorPointForPosition(false);
	}
	layer->setAnchorPoint(world->getAnchorPoint());
	layer->setPosition(world->getPosition());
	this->addChild(layer, -1);
	auto menu_back = MenuItemFont::create(get_UTF8_string("back"), [this](Ref *sender) {
		auto next_scene = MainMenu::createScene();
		auto Transition_scene = TransitionCrossFade::create(SCENE_TURN_TRANSITION_TIME, next_scene);
		Director::getInstance()->replaceScene(Transition_scene);
	});
	menu_back->setAnchorPoint(Vec2::ZERO);
	auto menu = Menu::create(menu_back, NULL);
	menu->setAnchorPoint(Vec2::ZERO);
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu);

	auto listener_touch = EventListenerTouchOneByOne::create();
	Vec2 *touch_begin = new Vec2();
	listener_touch->onTouchBegan = [this, touch_begin](Touch *t, Event *e) {
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
	auto go_with_dir = [this](DIRECTION dir) {
		if (!world->is_empty(world->get_next_position(player->get_position(), dir)) || abs(dir - player->get_current_dir()) == 2) {
			return;
		}
		player->turn(dir);
		player->go_ahead();
		if (game_map.count(player->get_position()) && game_map[player->get_position()].length() > 3) {
			user_info["game_menu_nxt_pos_x"] = player->get_position().first;
			user_info["game_menu_nxt_pos_y"] = player->get_position().second;
			user_info["game_menu_nxt_dir"] = player->get_current_dir();
			auto next_scene = MyGame::createScene(game_map[player->get_position()]);
			auto Transition_scene = TransitionCrossFade::create(SCENE_TURN_TRANSITION_TIME, next_scene);
			Director::getInstance()->replaceScene(Transition_scene);
			//Director::getInstance()->pushScene(Transition_scene);
		}
	};
	listener_touch->onTouchEnded = [this, set_dir, go_with_dir](Touch *t, Event *e) {
		if (Rect(Vec2::ZERO, world->getContentSize()).containsPoint(world->convertToNodeSpace(t->getStartLocation()))) {
			auto control_mode = user_info["control_mode"].asInt();
			if (control_mode == 0 || control_mode == 2) {
				auto v = t->getLocation() - t->getStartLocation();
				if (v.isZero()) {
					v = t->getLocation() - (origin + visible_size / 2);
					DIRECTION dir = set_dir(v);
					go_with_dir(dir);
				}
			}
		}
		return true;
	};
	listener_touch->onTouchMoved = [this, set_dir, touch_begin, go_with_dir](Touch *t, Event *e) {
		auto control_mode = user_info["control_mode"].asInt();
		if (control_mode == 0 || control_mode == 1) {
			auto pos = t->getLocation();
			auto start_pre = t->getPreviousLocation() - *touch_begin;
			auto pre_now = pos - t->getPreviousLocation();
			if (!start_pre.isZero() && abs(Vec2::angle(start_pre, pre_now)) > acos(-1.0f) / 4) {
				*touch_begin = pos;
			}
			if (fabs(pos.x - touch_begin->x) > touch_move_len || fabs(pos.y - touch_begin->y) > touch_move_len) {
				DIRECTION dir = set_dir(pos - *touch_begin);
				go_with_dir(dir);
				*touch_begin = pos;
			}
		}
	};
	auto listener_key = EventListenerKeyboard::create();
	listener_key->onKeyPressed = [this, go_with_dir](EventKeyboard::KeyCode key, Event *e) {
		DIRECTION dir;
		switch (key) {
		case EventKeyboard::KeyCode::KEY_UP_ARROW:
		case EventKeyboard::KeyCode::KEY_W:
		case EventKeyboard::KeyCode::KEY_CAPITAL_W:
			dir = DIRECTION::UP;
			break;
		case EventKeyboard::KeyCode::KEY_DOWN_ARROW:
		case EventKeyboard::KeyCode::KEY_S:
		case EventKeyboard::KeyCode::KEY_CAPITAL_S:
			dir = DIRECTION::DOWN;
			break;
		case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
		case EventKeyboard::KeyCode::KEY_A:
		case EventKeyboard::KeyCode::KEY_CAPITAL_A:
			dir = DIRECTION::LEFT;
			break;
		case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
		case EventKeyboard::KeyCode::KEY_D:
		case EventKeyboard::KeyCode::KEY_CAPITAL_D:
			dir = DIRECTION::RIGHT;
			break;
		default:
			return;
		}
		go_with_dir(dir);
	};
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener_touch, this);
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener_key, this);
	return true;
}
