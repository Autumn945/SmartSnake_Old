#pragma once
#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "AppDelegate.h"
#include "HelloWorldScene.h"
#include "MyUtility.h"
#include "MainMenuScene.h"

#define UNIT 16

#define MAIN_MENU_TURN_TIME 0.5f
#define SCENE_TURN_DELAY 0.1f
#define SCENE_TURN_TRANSITION_TIME 0.3f

#define CREATE_MAIN_MENU_ACTIONS \
	auto ac_rotate_to_n90 = RotateTo::create(MAIN_MENU_TURN_TIME, Vec3(0, -90, 0));\
	auto ac_rotate_to_90 = RotateTo::create(MAIN_MENU_TURN_TIME, Vec3(0, 90, 0));\
	auto ac_rotate_to_0 = RotateTo::create(MAIN_MENU_TURN_TIME, Vec3(0, 0, 0));\
	auto ac_fade_out = FadeOut::create(MAIN_MENU_TURN_TIME);\
	auto ac_fade_in = FadeIn::create(MAIN_MENU_TURN_TIME);\
	auto ac_move_to_mid = MoveTo::create(MAIN_MENU_TURN_TIME, position);\
	auto ac_move_to_left = MoveTo::create(MAIN_MENU_TURN_TIME, position - Vec2(width / 2, 0));\
	auto ac_move_to_right = MoveTo::create(MAIN_MENU_TURN_TIME, position + Vec2(width / 2, 0));\
	auto turn_right = Sequence::create(Spawn::create(ac_rotate_to_90, ac_move_to_right, ac_fade_out, NULL), CallFuncN::create([](Ref *sender) {\
		((Node *)sender)->setVisible(false);\
	}), NULL);\
	auto turn_mid = Sequence::create(CallFuncN::create([](Ref *sender) {\
		((Node *)sender)->setVisible(true);\
	}), Spawn::create(ac_rotate_to_0, ac_move_to_mid, ac_fade_in, NULL), NULL);\
	auto turn_left = Sequence::create(Spawn::create(ac_rotate_to_n90, ac_move_to_left, ac_fade_out, NULL), CallFuncN::create([](Ref *sender) {\
		((Node *)sender)->setVisible(false);\
	}), NULL)

#define CREATE_MANE_ITEM(name) \
auto name = MenuItemFont::create(MyUtility::get_UTF8_string(#name), [=](Ref *sender) {\
		log(#name);\
		deal_menu_event(#name);\
	})
