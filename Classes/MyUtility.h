#pragma once
#include "SystemHeader.h"
using namespace std;
USING_NS_CC;
class MyUtility {
public:
	static string get_UTF8_string(string key);
	static int get_int(string key);
	static float get_float(string key);
	static double get_double(string key);
	static bool init();
	static bool deal_with_event(string event_name);

	static ValueMap value_map;
	static Size visible_size;
	static Vec2 origin;
};

