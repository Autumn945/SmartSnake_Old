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
};
