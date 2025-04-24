#include "Outputs.h"


// This is the magic function that just allocates a bit
// It can allocate indices 1-48 (inclusive)
// It allocates like this (all inclusive):
// 1-21: direct to digital buttons
// 22-24: encodes it to POV
// 25-32: encodes it into analog axis 3
// 33-40: encodes it into analog axis 4
// 41-48: encodes it into analog axis 5
void writeOut(int ind, bool val) {
  int curr_max = digital_outputs;
  if (ind <= curr_max) {
    Joystick.button(ind, val);
  }
  curr_max += pov_bits;
  if (ind <= curr_max) {
    povcache[ind - curr_max + pov_bits - 1] = val;
  }
  curr_max += analog_bits;
  if (ind <= curr_max) {
    cache3[ind - curr_max + analog_bits - 1] = val;
  }
  curr_max += analog_bits;
  if (ind <= curr_max) {
    cache4[ind - curr_max + analog_bits - 1] = val;
  }
  curr_max += analog_bits;
  if (ind <= curr_max) {
    cache5[ind - curr_max + analog_bits - 1] = val;
  }
}

void writeCaches() {
  int val = 0;
  if (povcache[0]) {
    val += 45;
  }
  if (povcache[1]) {
    val += 90;
  }
  if (povcache[2]) {
    val += 180;
  }
  Joystick.hat(val);
  Joystick.use8bit(true);
  Serial.println(encode8ToConstrainedInt(cache5));
  Joystick.sliderRight(encode8ToConstrainedInt(cache5));
  Joystick.sliderLeft(encode8ToConstrainedInt(cache4));
  Joystick.Zrotate(encode8ToConstrainedInt(cache3));
}