#include "SystemHeader.h"

ValueMap value_map;
Size visible_size;
Vec2 origin;

int UNIT;
float MAIN_MENU_TURN_TIME;
float SCENE_TURN_DELAY;
float SCENE_TURN_TRANSITION_TIME;
int TOP_LABEL_FONT_SIZE;
int DEFAULT_MENU_FONT_SIZE;
int DEFAULT_LABEL_FONT_SIZE;


bool init() {
	log("init!");
	value_map = FileUtils::getInstance()->getValueMapFromFile("value_map.xml");
	log("init 2");
	if (value_map.size() == 0) {
		log("!!!!!!!!!!!!!!!!!!getValueMapFromFile value_map.xml failed");
		return false;
	}
	visible_size = Director::getInstance()->getVisibleSize();
	origin = Director::getInstance()->getVisibleOrigin();
	UNIT = get_int("unit");
	MAIN_MENU_TURN_TIME = get_float("main_menu_turn_time");
	SCENE_TURN_DELAY = get_float("scene_turn_delay");
	SCENE_TURN_TRANSITION_TIME = get_float("scene_turn_transition_time");
	TOP_LABEL_FONT_SIZE = get_int("top_label_font_size");
	DEFAULT_MENU_FONT_SIZE = get_int("default_menu_font_size");
	DEFAULT_LABEL_FONT_SIZE = get_int("default_label_font_size");
	return true;
}


int sign(float x) {
	if (x > EPS) {
		return 1;
	}
	if (x < -EPS) {
		return -1;
	}
	return 0;
}

int float_to_int(float f) {
	f += sign(f) * 0.5;
	return (int)f;
}

Value get_value(string key) {
	if (value_map.count(key) == 0) {
		log("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! key %s have not defined!", key.c_str());
		return Value("unknown");
	}
	return value_map[key];
}

string get_UTF8_string(string key) {
	auto ret = get_value(key).asString();
	if (ret.length() == 0) {
		ret = "undefinition";
	}
	return ret;
}

int get_int(string key) {
	auto ret = get_value(key).asInt();
	return ret;
}

float get_float(string key) {
	auto ret = get_value(key).asFloat();
	return ret;
}
double get_double(string key) {
	auto ret = get_value(key).asDouble();
	return ret;
}