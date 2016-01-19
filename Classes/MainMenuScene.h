#pragma once
#include "SystemHeader.h"
USING_NS_CC;
using namespace std;
class MainMenu :
	public Layer {
public:
	// there's no 'id' in cpp, so we recommend returning the class instance pointer
	static cocos2d::Scene* createScene();

	// Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
	virtual bool init();

	// implement the "static create()" method manually
	CREATE_FUNC(MainMenu);

	int menu_id;
	Vec2 position;
	float width;
	vector<MenuItem *> vector_menu;
	void deal_menu_event(string menu);
};

