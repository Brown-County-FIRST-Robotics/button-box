#include <vector>

#include "i2c_boards.h"


// IMPORTANT NOTE: Dinput button and axis ids are from 1-128 and 1-8 respectively in this program so that 0 can be used as "null" (pin has no binding)
// They are decremented by 1 to output to buttons 0-127 in DInput
std::array<I2cBoard*, 3> allModules = {
  new MCP23017("Oli Industrial Box", 0x20, {1, 2, 3, 4, 5, 6}),
  new ADS7830("Cool Joystick", 0x48, {}),
  new ADS1015("Accurate Analog", 0x49, {1, 2})
};

std::vector<int> activeModules; //list of indices of allModules that are active

void setup() {
  Serial.begin(9600);
  while (!Serial) delay(10);

  checkActiveModules();
}

void loop() {
  for (int i : activeModules) {
    allModules[i]->update();
  }
}

void checkActiveModules() { //sets activeModules to the current button box sutup
  activeModules.clear();

  for (int i = 0; i < allModules.size(); i++) {
    if (allModules[i]->initialize()) {
      activeModules.push_back(i);
    }
  }
}
