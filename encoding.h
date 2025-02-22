#pragma once
#include <iostream>
#include <vector>
#include <map>
#include <cmath>



// Note: trit is just like bit, but base-3


// Converts a bunch of boolean pairs where they are not both true to an integer
// It works by converting the states to base-3, then outputting an integer
// Encodes it in stupid little endian
int PoolMutexEntropy(std::vector<std::pair<bool,bool>> information) {
	int ret=0;
	// 00:0
	// 01:1
	// 10:2
	for(int i=0; i<information.size(); i++) {
		int trit=0; // the trit at this position
		// This should not be true, as that would violate the requirements for this function
		if(information[i].first&&information[i].second) {
			throw std::invalid_argument("Information["+std::to_string(i)+"] violates mutex rule");
		}
		if(!information[i].first&&!information[i].second) {
			trit=0;
		}
		if(information[i].first) {
			trit=2;
		}
		if(information[i].second) {
			trit=1;
		}
		int exponent=std::pow(3,i);
		ret+=trit*exponent;
	}
	// This is done to reserve 0 as a "null state", such as not being plugged in.
	ret=ret+1;
	return ret;
}

std::vector<std::pair<bool,bool>> SplitMutexEntropy(int inp, int size) {
	// Check if this is the "null state" (ie: not plugged in)
	if(inp==0) {
		return {};
	}
	inp-=1;// Reverse null state adjustment
	std::vector<std::pair<bool,bool>> ret;
	for(int i=0; i<size; i++) {
		int trit=inp%3;// Get the first bit
		inp/=3; // Shift to the next bit
		std::pair<bool,bool> translated_bit{false,false};
		if(trit==1) {
			translated_bit.second=true;
		}
		if(trit==2) {
			translated_bit.first=true;
		}
		ret.push_back(translated_bit);
	}
	return ret;
}

// converts 8 booleans to an int
int32_t encode8ToConstrainedInt(std::vector<bool> vals) {
	int32_t out=0;
	for(int i=0; i<vals.size(); i++) {
		bool bit=vals[i];
		int exponent=std::pow(2,i);
		if(bit) {
			out+=exponent;
		}
	}
	return out-128;
}


// Port to Java
// Converts a float from -1 to 1 to 8 booleans
std::vector<bool> deencodeFloatToVec(float f) {
	int val=0;
	if(f<0) {
		val=f*128;
	} else {
		val=f*127;
	}
	val+=128;
	std::vector<bool> ret;
	for(int i=0; i<8; i++) {
		ret.push_back(val&((int) std::pow(2,i)));
	}
	return ret;
}


// From wpilib: hal/src/main/native/athena/FRCDriverStation.cpp 74-82
// Testing function, simulates what the driver station does
float mockConverter(int32_t val) {
	int8_t value=val;
	if(value<0) {
		return value/128.0;
	} else {
		return value/127.0;
	}
};