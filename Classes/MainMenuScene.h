#pragma once
#include "SystemHeader.h"

USING_NS_CC;
using namespace std;
class MainMenu : public Layer {
public:
	static Scene* createScene();
	virtual bool init();
	CREATE_FUNC(MainMenu);

	int menu_id;
	Vec2 position;
	float menu_item_width;
	vector<MenuItem *> vector_menu;
	bool deal_with_event(string event_name);
};
