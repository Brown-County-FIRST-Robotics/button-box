#include <vector>
#include <Joystick.h>

#include "i2c_boards.h"

struct Module {
  std::string name;
  std::vector<I2cBoard*> boards;
};

// IMPORTANT NOTE: Dinput button and axis ids are from 1-128 and 1-8 respectively in this program so that 0 can be used as "null" (pin has no binding)
// They are decremented by 1 to output to buttons 0-127 in DInput
std::vector<Module> allModules = {
  {"Oli Industrial Box", {new MCP23017(0x20, {1, 2, 3, 4, 5, 6})}},
  {"Fancy Joystick", {new ADS7830(0x48, {})}},
  {"Accurate Analog", {new ADS1015(0x49, {1, 2})}}
};

std::vector<int> activeModules; //list of indices of allModules that are active

void setup() {
  Serial.begin(9600);
  while (!Serial) delay(10);

  Joystick.begin();

  checkActiveModules();
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

  Serial.println("Active modules:");
  for (int i : activeModules) {
    Serial.println(" " + String(allModules[i].name.c_str()));
  }
}
