#include "MyGameScene.h"
#include "MissionSprite.h"
#include "Snake.h"
#include "SmartSnake.h"
#include "GameMenuScene.h"

USING_NS_CC;
using namespace std;

MyGame * MyGame::create(int mission_id) {
	MyGame *pRet = new(std::nothrow) MyGame();
	if (pRet && pRet->init(mission_id)) {
		pRet->autorelease();
		return pRet;
	}
	else {
		delete pRet;
		pRet = nullptr;
		return nullptr;
	}
	return nullptr;
}

Scene* MyGame::createScene(int mission_id) {
	auto scene = Scene::create();
	auto layer = MyGame::create(mission_id);
	scene->addChild(layer);
	return scene;
}

bool MyGame::init(int mission_id) {
	if (!Layer::init()) {
		return false;
	}
	log("my game init");
	auto mission = Mission::create(mission_id);
	min_score = mission->flower[0];
	score = 0;
	this->setTag(mission_id);
	game_map = mission->get_game_map();
	game_map->setTag(mission_id);
	log("%d", game_map);
	this->addChild(game_map);
	log("test");
	auto bug_v = game_map->getProperty("bug");
	auto flower_v = game_map->getProperty("flower");
	if (bug_v.isNull()) {
		bug_v = 0;
	}
	if (flower_v.isNull()) {
		flower_v = 0;
	}
	bug = bug_v.asInt();
	flower = flower_v.asInt();
	for (int i = 0; i < foods_num; i++) {
		int gid = i + 1;
		auto v = game_map->getPropertiesForGID(gid).asValueMap();
		if (v.count("cooldown") == 0) {
			v["cooldown"] = 0;
		}
		cooldown[i] = v["cooldown"].asInt();
		current_cooldown[i] = cooldown[i];
	}
	auto snake_obj_group = game_map->getObjectGroup("snake_objs");
	CCASSERT(snake_obj_group, "snake_objs has not defined!");
	auto snake_objs = snake_obj_group->getObjects();
	snakes.clear();
	snakes.push_back(Snake::create("player", game_map));
	CCASSERT(snakes[0], "snakes[0] has not defined!");
	for (auto snake_v : snake_objs) {
		auto snake_vm = snake_v.asValueMap();
		auto snake_name = snake_vm["name"].asString();
		if (snake_name != "player") {
			auto snake_type = snake_vm["type"].asString();
			if (snake_type == "player") {
				snakes.push_back(Snake::create(snake_name, game_map));
			}
			else {
				auto snake = SmartSnake::create(snake_name, game_map);
				if (snake_type == "follow") {
					snake->type = Snake::SnakeType::t_follow;
				}
				else {
					snake->type = Snake::SnakeType::t_enemy;
				}
				snakes.push_back(snake);
			}
		}
	}
	snakes[0]->setUserObject(this);
	//game_map->addChild(snakes[0]);
	auto label_score = Label::createWithSystemFont("score: 0", "Arial", SMALL_LABEL_FONT_SIZE);
	label_score->setAnchorPoint(Vec2(0.5, 0));
	label_score->setPosition(this->getContentSize().width / 2, 0);
	label_score->setName("score");
	this->addChild(label_score);

	auto label_goal = Label::createWithSystemFont(get_UTF8_string("goal"), "Arial", MID_LABEL_FONT_SIZE);
	label_goal->setAnchorPoint(Vec2(0, 1));
	label_goal->setPosition(origin + Vec2(0, visible_size.height));
	this->addChild(label_goal);

	scheduleUpdate();

	auto layer = LayerColor::create(Color4B::GRAY, game_map->getContentSize().width, game_map->getContentSize().height);
	if (layer->isIgnoreAnchorPointForPosition()) {
		layer->ignoreAnchorPointForPosition(false);
	}
	layer->setAnchorPoint(game_map->getAnchorPoint());
	layer->setPosition(game_map->getPosition());
	this->addChild(layer, -1);

	auto menu_back = MenuItemFont::create(get_UTF8_string("back"), [this](Ref *sender) {
		auto next_scene = GameMenu::createScene();
		auto Transition_scene = TransitionCrossFade::create(SCENE_TURN_TRANSITION_TIME, next_scene);
		Director::getInstance()->replaceScene(Transition_scene);
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
			auto control_mode = user_info["control_mode"].asInt();
			if (control_mode == 0 || control_mode == 2) {
				auto v = t->getLocation() - t->getStartLocation();
				if (v.isZero()) {
					v = t->getLocation() - (origin + visible_size / 2);
					DIRECTION dir = set_dir(v);
					for (auto snake : snakes) {
						auto snake_type = snake->get_type();
						if (snake_type == Snake::SnakeType::t_player || snake_type == Snake::SnakeType::t_follow) {
							snake->turn(dir);
						}
					}
				}
			}
		}
		return true;
	};
	listener_touch->onTouchMoved = [this, set_dir, touch_begin](Touch *t, Event *e) {
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
				for (auto snake : snakes) {
					auto snake_type = snake->get_type();
					if (snake_type == Snake::SnakeType::t_player || snake_type == Snake::SnakeType::t_follow) {
						snake->turn(dir);
					}
				}
				*touch_begin = pos;
			}
		}
	};
	auto listener_key = EventListenerKeyboard::create();
	listener_key->onKeyPressed = [this](EventKeyboard::KeyCode key, Event *e) {
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
		snakes[0]->turn(dir);
	};
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener_touch, this);
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener_key, this);
	return true;
}

void MyGame::update(float dt) {
	auto fps = Director::getInstance()->getAnimationInterval();
	auto food_layer = game_map->getLayer("food");
	CCASSERT(food_layer, "food_layer has not defined!");
	for (int i = 0; i < foods_num; i++) {
		if (cooldown[i] > 0) {
			current_cooldown[i] -= fps;
			if (current_cooldown[i] < 0) {
				current_cooldown[i] = cooldown[i];
				auto pos = game_map->get_random_empty_point();
				if (pos.first >= 0) {
					food_layer->setTileGID(i + 1, Vec2(pos.first, pos.second));
				}
			}
		}
	}
	for (auto snake : snakes) {
		snake->go_step();
	}
	for (auto snake : snakes) {
		if (!snake->get_is_checked()) {
			snake->check();
		}
	}
	auto label = (Label*)this->getChildByName("score");
	label->setString("score: " + Value(get_score()).asString());
}

void MyGame::game_over() {
	string id_string = Value(this->getTag()).asString();
	if (get_score() >= min_score && bug == 0 && flower == 0) {
		user_info["mission_score" + id_string] = max(get_score(), user_info["mission_score" + id_string].asInt());
		user_info["mission_success" + id_string] = user_info["mission_success" + id_string].asInt() + 1;
	}
	user_info["mission_challenge" + id_string] = user_info["mission_challenge" + id_string].asInt() + 1;
	FileUtils::getInstance()->writeValueMapToFile(user_info, "user_info.xml");
	auto next_scene = GameMenu::createScene();
	auto Transition_scene = TransitionCrossFade::create(SCENE_TURN_TRANSITION_TIME, next_scene);
	Director::getInstance()->replaceScene(Transition_scene);
}
