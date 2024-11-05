#include <vector>
#include <Joystick.h>

#include "i2c_boards.h"
struct Module {
  std::string name;
  std::vector<I2cBoard*> boards;
};

// DInput controller, currently the number of analog axes must be set manually
Joystick_ dInput{};
  //128, 0,                // Button Count, POV Count
  //true, true, false,     // LX, LY, LZ Axis active //NOTE: there appears to be 10 analog axes in this library, but dInput supports only 8, requires further testing
 // false, false, false,   // RX, RY, RZ Axis active
//  false, false          // "rudder", "throttle" active
// );  // "accelerator", "brake", "steering" active

// IMPORTANT NOTE: Dinput button and axis ids are from 1-128 and 1-8 respectively in this program so that 0 can be used as "null" (pin has no binding)
// They are decremented by 1 to output to buttons 0-127 in DInput
std::vector<Module> allModules = {
  {"Oli Industrial Box", {new MCP23017(0x20, {2,3,4,5,6,7,8,9})}},
  {"Oli Industrial Box2", {new MCP23017(0x23, {10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25})}},
  {"Fancy Joystick", {new ADS7830(0x48, {})}},
  {"Accurate Analog", {new ADS1015(0x49, {1, 2})}}
};

std::vector<int> activeModules; //list of indices of allModules that are active

void setup() {
  Serial.begin(9600);
  while (!Serial) delay(10);

  checkActiveModules();

  // joystick initialization
  Joystick.begin();
}

void loop() {
  for (int i : activeModules) {
    for (I2cBoard* board : allModules[i].boards) {
      board->update();
    }
  }
}

void checkActiveModules() { //sets activeModules to the current button box sutup
  activeModules.clear();

  for (int i = 0; i < allModules.size(); i++) {
    if (allModules[i].boards[0]->initialize()) { //assumes that the module is connected if its first board is connected
      activeModules.push_back(i);
      for (int j = 1; j < allModules[i].boards.size(); j++) { //initialize the rest of the boards
        allModules[i].boards[j]->initialize();
      }
    }
  }

  

  // Serial.println("Active modules:");
  // for (int i : activeModules) {
  //   Serial.println(" " + allModules[i].name);
  // }
}

