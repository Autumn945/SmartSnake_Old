#pragma once
#include "SystemHeader.h"
using namespace std;
USING_NS_CC;
class MyUtility {
public:
	static ValueMap value_map;
	static Size visible_size;
	static Vec2 origin;

	static string get_UTF8_string(string key);
	static bool init();
};

