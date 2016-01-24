#include "HelloWorldScene.h"

USING_NS_CC;
using namespace std;

Scene* HelloWorld::createScene() {
	auto scene = Scene::create();
	auto layer = HelloWorld::create();
	scene->addChild(layer);
	return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
	log("helloworld init");
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() ) {
        return false;
    }
	if (!::init()) {
		return false;
	}
	//create background
	auto center = origin + visible_size / 2;
	auto sp = Sprite::create(get_UTF8_string("file_helloworld"));
	sp->setPosition(center);
	this->addChild(sp);
	//init default font size
	MenuItemFont::setFontSize(DEFAULT_MENU_FONT_SIZE);
	log("Start = %s.", get_UTF8_string("Start").c_str());
	schedule(schedule_selector(HelloWorld::delay_call), 0, 0, SCENE_TURN_DELAY);

	Director::getInstance()->getTextureCache()->addImage(get_UTF8_string("file_wall"));
	Director::getInstance()->getTextureCache()->addImage(get_UTF8_string("file_hole"));
	Director::getInstance()->getTextureCache()->addImage(get_UTF8_string("file_players_head"));
	Director::getInstance()->getTextureCache()->addImage(get_UTF8_string("file_players_body"));
	Director::getInstance()->getTextureCache()->addImage(get_UTF8_string("file_players_body_turn"));
	Director::getInstance()->getTextureCache()->addImage(get_UTF8_string("file_players_tail"));

    return true;
}

void HelloWorld::delay_call(float dt) {
	TURN_TO_NEXT_SCENE_WITH_NAME(MainMenu);
	log("scene replaced");
}
