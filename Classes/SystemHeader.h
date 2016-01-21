#pragma once


#define UNIT MyUtility::get_int("unit")
#define MAIN_MENU_TURN_TIME MyUtility::get_float("main_menu_turn_time")
#define SCENE_TURN_DELAY MyUtility::get_float("scene_turn_delay")
#define SCENE_TURN_TRANSITION_TIME MyUtility::get_float("scene_turn_transition_time")
#define TOP_LABEL_FONT_SIZE MyUtility::get_int("top_label_font_size")
#define DEFAULT_MENU_FONT_SIZE MyUtility::get_int("default_menu_font_size")
#define DEFAULT_LABEL_FONT_SIZE MyUtility::get_int("default_label_font_size")

#define DIR_MASK 3

//key is a string
#define STRING(key) MyUtility::get_UTF8_string(key)
#define INTEGER(key) MyUtility::get_int(key)
#define FLOAT(key) MyUtility::get_float(key)

//name is the menu and event name
#define CREATE_MENU_ITEM_WITH_NAME(name) \
	auto name = MenuItemFont::create(MyUtility::get_UTF8_string(#name), [=](Ref *sender) {\
		log(#name);\
		if(!deal_with_event(#name)) {\
			log(#name"have not dealt!");\
		}\
	})

//name is the scene class name
#define TURN_TO_NEXT_SCENE_WITH_NAME(name) \
	auto next_scene = name::createScene();\
	auto Transition_scene = TransitionCrossFade::create(SCENE_TURN_TRANSITION_TIME, next_scene);\
	Director::getInstance()->replaceScene(Transition_scene)


#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "AppDelegate.h"
#include "MyUtility.h"
#include "MainMenuScene.h"
#include "HelloWorldScene.h"
#include "GameMenuScene.h"
#include "StorageScene.h"
#include "OptionScene.h"
#include "MyGameScene.h"