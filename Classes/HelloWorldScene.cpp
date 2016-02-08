#include "HelloWorldScene.h"

USING_NS_CC;
using namespace std;

Scene* HelloWorld::createScene() {
	auto scene = Scene::create();
	auto layer = HelloWorld::create();
	if (layer == NULL) {
		log("layer of HelloWorld creating failed!");
	}
	else {
		scene->addChild(layer);
	}
	return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init() {
	log("helloworld init");
    // super init first
    if ( !Layer::init() ) {
        return false;
    }
	// global init
	if (!::init()) {
		return false;
	}
	// create background
	auto center = origin + visible_size / 2;
	auto sp = Sprite::create("HelloWorld.png");
	if (sp == NULL) {
		log("HelloWorld.png has not found");
		return false;
	}
	sp->setPosition(center);
	this->addChild(sp);

	// init default font size
	MenuItemFont::setFontSize(DEFAULT_MENU_FONT_SIZE);

	// to next scene
	schedule([](float dt) {
		auto next_scene = MainMenu::createScene();
		if (next_scene == NULL) {
			log("scene of HelloWorld creating failed!");
			return;
		}
		auto Transition_scene = TransitionCrossFade::create(SCENE_TURN_TRANSITION_TIME, next_scene);
		Director::getInstance()->replaceScene(Transition_scene);
		log("scene replaced");
	}, 0, 0, SCENE_TURN_DELAY, "replace scene");
    return true;
}