#include "HelloWorldScene.h"

USING_NS_CC;

using namespace cocostudio::timeline;

Scene* HelloWorld::createScene() {
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = HelloWorld::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() ) {
        return false;
    }
	if (!MyUtility::init()) {
		return false;
	}
	auto center = MyUtility::origin + MyUtility::visible_size / 2;
	auto sp = Sprite::create(MyUtility::get_UTF8_string("file_helloworld"));
	sp->setPosition(center);
	this->addChild(sp);
	log("Start = %s.", MyUtility::get_UTF8_string("Start").c_str());
	schedule(schedule_selector(HelloWorld::delay_call), 0, 0, SCENE_TURN_DELAY);

    return true;
}

void HelloWorld::delay_call(float dt) {
	auto next_scene = MainMenu::createScene();
	auto Transition_scene = TransitionCrossFade::create(SCENE_TURN_TRANSITION_TIME, next_scene);
	Director::getInstance()->replaceScene(Transition_scene);
	log("scene replaced");
}
