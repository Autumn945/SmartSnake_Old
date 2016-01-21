#include "MainMenuScene.h"

#define CREATE_MAIN_MENU_ACTIONS \
	auto ac_rotate_to_n90 = RotateTo::create(MAIN_MENU_TURN_TIME, Vec3(0, -90, 0));\
	auto ac_rotate_to_90 = RotateTo::create(MAIN_MENU_TURN_TIME, Vec3(0, 90, 0));\
	auto ac_rotate_to_0 = RotateTo::create(MAIN_MENU_TURN_TIME, Vec3(0, 0, 0));\
	auto ac_fade_out = FadeOut::create(MAIN_MENU_TURN_TIME);\
	auto ac_fade_in = FadeIn::create(MAIN_MENU_TURN_TIME);\
	auto ac_move_to_mid = MoveTo::create(MAIN_MENU_TURN_TIME, position);\
	auto ac_move_to_left = MoveTo::create(MAIN_MENU_TURN_TIME, position - Vec2(menu_item_width / 2, 0));\
	auto ac_move_to_right = MoveTo::create(MAIN_MENU_TURN_TIME, position + Vec2(menu_item_width / 2, 0));\
	auto turn_right = Sequence::create(Spawn::create(ac_rotate_to_90, ac_move_to_right, ac_fade_out, NULL), CallFuncN::create([](Ref *sender) {\
		((Node *)sender)->setVisible(false);\
	}), NULL);\
	auto turn_mid = Sequence::create(CallFuncN::create([](Ref *sender) {\
		((Node *)sender)->setVisible(true);\
	}), Spawn::create(ac_rotate_to_0, ac_move_to_mid, ac_fade_in, NULL), NULL);\
	auto turn_left = Sequence::create(Spawn::create(ac_rotate_to_n90, ac_move_to_left, ac_fade_out, NULL), CallFuncN::create([](Ref *sender) {\
		((Node *)sender)->setVisible(false);\
	}), NULL)

USING_NS_CC;
using namespace std;

Scene* MainMenu::createScene() {
	auto scene = Scene::create();
	auto layer = MainMenu::create();
	scene->addChild(layer);
	return scene;
}

bool MainMenu::init() {
	if (!Layer::init()) {
		return false;
	}

	log("MainMenu"" init");
	//(0.5, 1 - 0.618)
	position = Vec2(MyUtility::origin.x + MyUtility::visible_size.width / 2
		, MyUtility::origin.y + MyUtility::visible_size.height * (1 - 0.618));
	log("position = %f, %f", position.x, position.y);

	//main menu
	CREATE_MENU_ITEM_WITH_NAME(menu_start);
	CREATE_MENU_ITEM_WITH_NAME(menu_option_setting);
	CREATE_MENU_ITEM_WITH_NAME(menu_option_help);
	CREATE_MENU_ITEM_WITH_NAME(menu_option_about);
	CREATE_MENU_ITEM_WITH_NAME(menu_option_feedback);
	CREATE_MENU_ITEM_WITH_NAME(menu_exit);

	//add into a vector
	vector_menu.clear();
	vector_menu.push_back(menu_start);
	vector_menu.push_back(menu_option_setting);
	vector_menu.push_back(menu_option_help);
	vector_menu.push_back(menu_option_about);
	vector_menu.push_back(menu_option_feedback);
	vector_menu.push_back(menu_exit);

	//get max content width
	menu_item_width = vector_menu[0]->getContentSize().width;
	for (auto m : vector_menu) {
		menu_item_width = max(menu_item_width, m->getContentSize().width);
	}
	log("width = %f", menu_item_width);

	//add turn menu
	CREATE_MENU_ITEM_WITH_NAME(menu_turn_left);
	CREATE_MENU_ITEM_WITH_NAME(menu_turn_right);

	//create menu
	auto menu = Menu::create(menu_turn_left, menu_turn_right, NULL);
	menu->setPosition(Vec2::ZERO);
	menu->setAnchorPoint(Vec2::ZERO);

	menu_turn_left->setPosition(position - Vec2(menu_item_width / 2 + menu_turn_left->getContentSize().width / 2, 0));
	menu_turn_right->setPosition(position + Vec2(menu_item_width / 2 + menu_turn_right->getContentSize().width / 2, 0));
	this->addChild(menu);

	//init all menu itme
	for (auto m : vector_menu) {
		menu->addChild(m);
		m->setVisible(false);
	}

	//get action of turn menu
	CREATE_MAIN_MENU_ACTIONS;

	menu_id = 0;
	//init menu item
	vector_menu[menu_id]->setPosition(position);
	vector_menu[menu_id]->setRotation3D(Vec3(0, 0, 0));
	vector_menu[menu_id]->setOpacity(255);
	vector_menu[menu_id]->setVisible(true);

	return true;
}

bool MainMenu::deal_with_event(string event_name) {
	if (event_name == "menu_turn_left") {
		log("menu_turn_left!");
		log("menu_id = %d", menu_id);
		//get action of turn menu
		CREATE_MAIN_MENU_ACTIONS;

		//init menu item
		vector_menu[menu_id]->stopAllActions();
		vector_menu[menu_id]->setPosition(position);
		vector_menu[menu_id]->setRotation3D(Vec3(0, 0, 0));
		vector_menu[menu_id]->setOpacity(255);
		vector_menu[menu_id]->setVisible(true);
		//run action
		vector_menu[menu_id]->runAction(turn_left);
		//next menu id
		menu_id = (menu_id + 1) % vector_menu.size();
		//init menu item
		vector_menu[menu_id]->stopAllActions();
		vector_menu[menu_id]->setPosition(position + Vec2(menu_item_width / 2, 0));
		vector_menu[menu_id]->setRotation3D(Vec3(0, 90, 0));
		vector_menu[menu_id]->setOpacity(100);
		vector_menu[menu_id]->setVisible(false);
		//run action
		vector_menu[menu_id]->runAction(turn_mid);
		return true;
	}
	if (event_name == "menu_turn_right") {
		log("menu_turn_right!");
		log("menu_id = %d", menu_id);
		//get action of turn menu
		CREATE_MAIN_MENU_ACTIONS;

		//init menu item
		vector_menu[menu_id]->stopAllActions();
		vector_menu[menu_id]->setPosition(position);
		vector_menu[menu_id]->setRotation3D(Vec3(0, 0, 0));
		vector_menu[menu_id]->setOpacity(255);
		vector_menu[menu_id]->setVisible(true);
		//run action
		vector_menu[menu_id]->runAction(turn_right);
		//next menu id
		menu_id = (menu_id - 1 + vector_menu.size()) % vector_menu.size();
		//init menu item
		vector_menu[menu_id]->stopAllActions();
		vector_menu[menu_id]->setPosition(position - Vec2(menu_item_width / 2, 0));
		vector_menu[menu_id]->setRotation3D(Vec3(0, -90, 0));
		vector_menu[menu_id]->setOpacity(100);
		vector_menu[menu_id]->setVisible(false);
		//run action
		vector_menu[menu_id]->runAction(turn_mid);
		log("menu_id = %d", menu_id);
		return true;
	}
	if (event_name == "menu_start") {
		TURN_TO_NEXT_SCENE_WITH_NAME(MyGame);
		return true;
	}
	if (event_name.find("option") < event_name.length()) {
		auto next_scene = Option::createScene(event_name);
		auto Transition_scene = TransitionCrossFade::create(SCENE_TURN_TRANSITION_TIME, next_scene);
		Director::getInstance()->replaceScene(Transition_scene);
		return true;
	}
	if (MyUtility::deal_with_event(event_name)) {
		return true;
	}
	return false;
}
