#include "SystemHeader.h"

const float EPS = (float)(1e-4);
const float SCENE_TURN_DELAY = 0.2f;
const float MAIN_MENU_TURN_TIME = 0.4f;
const float SCENE_TURN_TRANSITION_TIME = 0.3f;

const int DIR_MASK = 3;
const int UNIT = 32;
const int TOP_LABEL_FONT_SIZE = 68;
const int DEFAULT_LABEL_FONT_SIZE = 30;
const int DEFAULT_MENU_FONT_SIZE = 48;
const int game_width = 31;
const int game_height = 19;

// origin is the left-up corner
// 0->up, 1->right, 2->down, 3->right
const pii dir_vector[4] = { pii(0, -1), pii(1, 0), pii(0, 1), pii(-1, 0) };

// string to chinese characters
ValueMap UTF8_string;
Size visible_size;
Vec2 origin;

bool init() {
	UTF8_string = FileUtils::getInstance()->getValueMapFromFile("UTF8_string.xml");
	if (UTF8_string.size() == 0) {
		log("!!!!!!!!!!!!!!!!!! size of UTF8_string is 0");
		return false;
	}
	
	visible_size = Director::getInstance()->getVisibleSize();
	origin = Director::getInstance()->getVisibleOrigin();
	return true;
}

// if x < 0 return -1, if x > 0 return 1, if x = 0 return 0
int sign(float x) {
	if (x > EPS) {
		return 1;
	}
	if (x < -EPS) {
		return -1;
	}
	return 0;
}

// get floor of float
int float_to_int(float f) {
	f += sign(f) * EPS;
	return (int)f;
}

Value get_value(ValueMap &value_map, string key) {
	if (value_map.count(key) == 0) {
		log("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! key %s have not defined!", key.c_str());
		return Value(key);
	}
	return value_map[key];
}

// return is string of UTF-8 string
string get_UTF8_string(string key) {
	return get_value(UTF8_string, key).asString();
}
