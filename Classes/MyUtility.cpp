#include "MyUtility.h"

ValueMap MyUtility::value_map;
Size MyUtility::visible_size;
Vec2 MyUtility::origin;

string MyUtility::get_UTF8_string(string key) {
	if (value_map.count(key) == 0) {
		log("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! key %s have not defined!", key.c_str());
		key = "undefinition";
	}
	auto ret = value_map[key].asString();
	if (ret.length() == 0) {
		ret = "undefinition";
	}
	return ret;
}

int MyUtility::get_int(string key) {
	if (value_map.count(key) == 0) {
		log("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! key %s have not defined!", key.c_str());
		return 0;
	}
	auto ret = value_map[key].asInt();
	return ret;
}

float MyUtility::get_float(string key) {
	if (value_map.count(key) == 0) {
		log("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! key %s have not defined!", key.c_str());
		return 0;
	}
	auto ret = value_map[key].asFloat();
	return ret;
}
double MyUtility::get_double(string key) {
	if (value_map.count(key) == 0) {
		log("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! key %s have not defined!", key.c_str());
		return 0;
	}
	auto ret = value_map[key].asDouble();
	return ret;
}

bool MyUtility::init() {
	value_map = FileUtils::getInstance()->getValueMapFromFile("value_map.xml");
	if (value_map.size() == 0) {
		log("!!!!!!!!!!!!!!!!!!getValueMapFromFile value_map.xml failed");
		return false;
	}
	visible_size = Director::getInstance()->getVisibleSize();
	origin = Director::getInstance()->getVisibleOrigin();
	return true;
}

bool MyUtility::deal_with_event(string event_name) {
	if (event_name == "menu_exit") {
		Director::getInstance()->end();
		return true;
	}
	if (event_name == "menu_back_to_main_menu") {
		TURN_TO_NEXT_SCENE_WITH_NAME(MainMenu);
		return true;
	}
	return false;
}