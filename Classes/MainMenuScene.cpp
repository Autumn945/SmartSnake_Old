#include "MainMenuScene.h"
USING_NS_CC;

Scene* MainMenu::createScene() {
	// 'scene' is an autorelease object
	auto scene = Scene::create();

	// 'layer' is an autorelease object
	auto layer = MainMenu::create();

	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}

bool MainMenu::init() {
	if (!Layer::init()) {
		return false;
	}

	log("MainMenu init");
	//(0.5, 1 - 0.618)
	position = Vec2(MyUtility::origin.x + MyUtility::visible_size.width / 2
		, MyUtility::origin.y + MyUtility::visible_size.height * (1 - 0.618));
	log("position = %f, %f", position.x, position.y);

	//main menu
	CREATE_MANE_ITEM(menu_start);
	CREATE_MANE_ITEM(menu_setting);
	CREATE_MANE_ITEM(menu_about);
	CREATE_MANE_ITEM(menu_feedback);
	CREATE_MANE_ITEM(menu_exit);

	//add into a vector
	vector_menu.clear();
	vector_menu.push_back(menu_start);
	vector_menu.push_back(menu_setting);
	vector_menu.push_back(menu_about);
	vector_menu.push_back(menu_feedback);
	vector_menu.push_back(menu_exit);

	//get max content width
	width = vector_menu[0]->getContentSize().width;
	for (auto m : vector_menu) {
		width = max(width, m->getContentSize().width);
	}
	log("width = %f", width);

	//add turn menu
	CREATE_MANE_ITEM(menu_turn_left);
	CREATE_MANE_ITEM(menu_turn_right);

	//create menu
	auto menu = Menu::create(menu_turn_left, menu_turn_right, NULL);
	menu->setPosition(Vec2::ZERO);
	menu->setAnchorPoint(Vec2::ZERO);

	menu_turn_left->setPosition(position - Vec2(width / 2 + menu_turn_left->getContentSize().width / 2, 0));
	menu_turn_right->setPosition(position + Vec2(width / 2 + menu_turn_right->getContentSize().width / 2, 0));
	this->addChild(menu);

	//init all menu itme
	for (auto m : vector_menu) {
		menu->addChild(m);
		m->setPosition(position + Vec2(width / 2, 0));
		m->setRotation3D(Vec3(0, 90, 0));
		m->setOpacity(0);
		m->setVisible(false);
	}

	//get action of turn menu
	CREATE_MAIN_MENU_ACTIONS;

	menu_id = 0;
	//run actions
	vector_menu[menu_id]->runAction(turn_mid);

	return true;
}

void MainMenu::deal_menu_event(string menu) {
	if (menu == "menu_turn_left") {
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
		vector_menu[menu_id]->setPosition(position + Vec2(width / 2, 0));
		vector_menu[menu_id]->setRotation3D(Vec3(0, 90, 0));
		vector_menu[menu_id]->setOpacity(100);
		vector_menu[menu_id]->setVisible(false);
		//run action
		vector_menu[menu_id]->runAction(turn_mid);
		return;
	}
	if (menu == "menu_turn_right") {
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
		vector_menu[menu_id]->setPosition(position - Vec2(width / 2, 0));
		vector_menu[menu_id]->setRotation3D(Vec3(0, -90, 0));
		vector_menu[menu_id]->setOpacity(100);
		vector_menu[menu_id]->setVisible(false);
		//run action
		vector_menu[menu_id]->runAction(turn_mid);
		log("menu_id = %d", menu_id);
		return;
	}
	if (menu == "menu_exit") {
		Director::getInstance()->end();
		return;
	}
}
