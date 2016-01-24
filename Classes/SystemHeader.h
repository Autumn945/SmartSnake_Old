#pragma once
#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"

#define DEFINE_VAR_GET_SET(varType, varName) \
	protected: varType varName;\
	public: varType get_##varName() {\
		return varName;\
	}\
	public: void set_##varName(varType var) {\
		varName = var;\
	}

#define DEFINE_VAR_SET(varType, varName) \
	protected: varType varName;\
	public: void set_##varName(varType var) {\
		varName = var;\
	}

#define DEFINE_VAR_GET(varType, varName) \
	protected: varType varName;\
	public: varType get_##varName() {\
		return varName;\
	}

#define DEFINE_VAR_GET_ADD(varType, varName) \
	protected: varType varName;\
	public: varType get_##varName() {\
		return varName;\
	}\
	public: void add_##varName(varType var) {\
		varName += var;\
	}


//name is the menu and event name
#define CREATE_MENU_ITEM_WITH_NAME(name) \
	auto name = MenuItemFont::create(get_UTF8_string(#name), [=](Ref *sender) {\
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

#define DEAL_WITH_EVENT \
	if (event_name == "menu_exit") {\
		Director::getInstance()->end();\
		return true;\
	}\
	if (event_name == "menu_back_to_main_menu") {\
		TURN_TO_NEXT_SCENE_WITH_NAME(MainMenu);\
		return true;\
	}


USING_NS_CC;
using namespace std;


enum DIRECTION {
	UP, RIGHT, DOWN, LEFT
};

extern bool init();
extern int sign(float x);
extern int float_to_int(float f);
extern Value get_value(string key);
extern string get_UTF8_string(string key);
extern int get_int(string key);
extern float get_float(string key);
extern double get_double(string key);

const int DIR_MASK = 3;
const float EPS = (float)(1e-4);
const Vec2 dir_vector[4] = { Vec2(0, 1), Vec2(1, 0), Vec2(0, -1), Vec2(-1, 0)};
extern ValueMap value_map;
extern Size visible_size;
extern Vec2 origin;

extern int UNIT;
extern float MAIN_MENU_TURN_TIME;
extern float SCENE_TURN_DELAY;
extern float SCENE_TURN_TRANSITION_TIME;
extern int TOP_LABEL_FONT_SIZE;
extern int DEFAULT_MENU_FONT_SIZE;
extern int DEFAULT_LABEL_FONT_SIZE;


#include "MyGameScene.h"
#include "GameNode.h"
#include "AppDelegate.h"
#include "HelloWorldScene.h"
#include "MainMenuScene.h"
#include "GameMenuScene.h"
#include "StorageScene.h"
#include "OptionScene.h"
#include "Snake.h"
