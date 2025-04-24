#pragma once
#include <iostream>
#include <vector>
#include <map>
#include <cmath>


// converts 8 booleans to an int
static int encode8ToConstrainedInt(std::vector<bool> vals) {
	int out = 0;
	for (int i = 0; i < vals.size(); i++) {
		bool bitt = vals[i];
		int exponent = std::pow(2, i);
		if (bitt) {
			out += exponent;
		}
	}
	return out - 128;
}

// Port to Java
// Converts a float from -1 to 1 to 8 booleans
static std::vector<bool> deencodeFloatToVec(float f) {
	int val = 0;
	if (f < 0) {
		val = f * 128;
	} else {
		val = f * 127;
	}
	val += 128;
	std::vector<bool> ret;
	for (int i = 0; i < 8; i++) {
		ret.push_back(val & ((int)std::pow(2, i)));
	}
	return ret;
}


// From wpilib: hal/src/main/native/athena/FRCDriverStation.cpp 74-82
// Testing function, simulates what the driver station does
static float mockConverter(int val) {
	int value = val;
	if (value < 0) {
		return value / 128.0;
	} else {
		return value / 127.0;
	}
};
