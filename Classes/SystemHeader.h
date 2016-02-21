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

USING_NS_CC;
using namespace std;

typedef pair<int, int> pii;
typedef map<pii, pii> mp_pii_pii;

bool init();
int sign(float x);
int float_to_int(float f);
string get_UTF8_string(string key);

enum DIRECTION {
	UP, RIGHT, DOWN, LEFT
};

extern const float EPS;
extern const float SCENE_TURN_DELAY;
extern const float MAIN_MENU_TURN_TIME;
extern const float SCENE_TURN_TRANSITION_TIME;

extern const int DIR_MASK;
extern const int UNIT;
extern const int TOP_LABEL_FONT_SIZE;
extern const int DEFAULT_LABEL_FONT_SIZE;
extern const int DEFAULT_MENU_FONT_SIZE;
extern const int touch_move_len;
const int max_game_width = 40;
const int max_game_height = 22;

extern const pii dir_vector[4];

extern ValueMap UTF8_string;
extern Size visible_size;
extern Vec2 origin;

#include "MyGameScene.h"
#include "GameMap.h"
#include "AppDelegate.h"
#include "HelloWorldScene.h"
#include "MainMenuScene.h"
#include "GameMenuScene.h"
#include "StorageScene.h"
#include "OptionScene.h"
#include "Snake.h"
