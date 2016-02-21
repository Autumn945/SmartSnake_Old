#include "OptionScene.h"

USING_NS_CC;
using namespace std; 

Option* Option::create(string name) {
	Option *pRet = new(std::nothrow) Option();
	if (pRet && (pRet->setName(name), pRet->init())) {
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
	auto menu_back = MenuItemFont::create(get_UTF8_string("back"), [](Ref *sender) {
		auto next_scene = MainMenu::createScene(); 
		auto Transition_scene = TransitionCrossFade::create(SCENE_TURN_TRANSITION_TIME, next_scene); 
		Director::getInstance()->replaceScene(Transition_scene);
	});
	menu_back->setAnchorPoint(Vec2(1, 0));
	menu_back->setPosition(origin.x + visible_size.width, origin.y);
	//create menu item
	auto menu = Menu::create(menu_back, NULL);
	menu->setAnchorPoint(Vec2::ZERO);
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu);
	//create top label
	auto top_label = Label::createWithSystemFont(get_UTF8_string(this->getName()), "Arial", TOP_LABEL_FONT_SIZE);
	top_label->setAnchorPoint(Vec2(0.5, 1));
	top_label->setPosition(origin.x + visible_size.width / 2
		, origin.y + visible_size.height); 
	this->addChild(top_label);
	// set context
	if (this->getName() == "setting") {
		return true;
	}
	if (this->getName() == "help") {
		return true;
	}
	if (this->getName() == "about") {
		return true;
	}
	if (this->getName() == "feedback") {
		return true;
	}
	//debug
	return true;
	return false;
}