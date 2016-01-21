#include "OptionScene.h"

USING_NS_CC;
using namespace std; 

Option* Option::create(string name) {
	Option *pRet = new(std::nothrow) Option();
	if (pRet) {
		pRet->setName(name);
	}
	if (pRet && pRet->init()) {
		pRet->autorelease();
		return pRet;
	}
	else {
		delete pRet;
		pRet = nullptr;
		return nullptr;
	}
}

Scene* Option::createScene(string name) {
	auto scene = Scene::create();
	auto layer = Option::create(name);
	scene->addChild(layer);
	return scene;
}

bool Option::init() {
	//////////////////////////////
	// 1. super init first
	if (!Layer::init()) {
		return false;
	}
	//add menu back_to_main_menu
	log("init option");
	CREATE_MENU_ITEM_WITH_NAME(menu_back_to_main_menu);
	menu_back_to_main_menu->setAnchorPoint(Vec2(1, 0));
	menu_back_to_main_menu->setPosition(MyUtility::origin.x + MyUtility::visible_size.width, MyUtility::origin.y);
	menu = Menu::create(menu_back_to_main_menu, NULL);
	menu->setAnchorPoint(Vec2::ZERO);
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu);
	log("add menu finished");

	auto top_label = LabelTTF::create(STRING(this->getName()), "Arial", TOP_LABEL_FONT_SIZE);
	top_label->setAnchorPoint(Vec2(0.5, 1));
	top_label->setPosition(MyUtility::origin.x + MyUtility::visible_size.width / 2
		, MyUtility::origin.y + MyUtility::visible_size.height); 
	this->addChild(top_label);
	if (this->getName() == "menu_option_setting") {
		return true;
	}
	if (this->getName() == "menu_option_help") {
		return true;
	}
	if (this->getName() == "menu_option_about") {
		return true;
	}
	if (this->getName() == "menu_option_feedback") {
		return true;
	}
	//debug
	return true;
	return false;
}

bool Option::deal_with_event(string event_name) {
	if (MyUtility::deal_with_event(event_name)) {
		return true;
	}
	return false;
}