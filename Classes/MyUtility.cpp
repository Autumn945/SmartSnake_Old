#include "MyUtility.h"

ValueMap MyUtility::value_map;
Size MyUtility::visible_size;
Vec2 MyUtility::origin;

string MyUtility::get_UTF8_string(string key) {
	auto ret = value_map[key].asString();
	if (ret.length() == 0) {
		ret = value_map["undefinition"].asString();
	}
	if (ret.length() == 0) {
		ret = "undefinition";
	}
	return ret;
}

bool MyUtility::init() {
	value_map = FileUtils::getInstance()->getValueMapFromFile("value_map.xml");
	if (value_map.size() == 0) {
		log("getValueMapFromFile value_map.xml failed");
		return false;
	}
	visible_size = Director::getInstance()->getVisibleSize();
	origin = Director::getInstance()->getVisibleOrigin();
	return true;
}
