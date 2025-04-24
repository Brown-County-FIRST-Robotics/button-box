#pragma once
#include <iostream>
#include <vector>
#include <Joystick.h>
#include "encoding.h"

static int digital_outputs = 21;
static int pov_bits = 3;
static int analog_bits = 8;

static std::vector<bool> povcache = { false, false, false };
static std::vector<bool> cache3 = { false, false, false, false, false, false, false, false };
static std::vector<bool> cache4 = { false, false, false, false, false, false, false, false };
static std::vector<bool> cache5 = { false, false, false, false, false, false, false, false };


// magic function
void writeOut(int ind, bool val);
void writeCaches();
