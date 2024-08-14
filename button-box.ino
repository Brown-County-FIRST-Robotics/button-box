// #include <PicoGamepad.h>
#include <vector>

#include "i2c_boards.h"

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
    allModules[i]->update(); //TODO: check what happens when an i2c device is disconnected and then the program attempts to read from it
  }
  //TODO: Add button to refresh modules, or refresh periodically
}

void checkActiveModules() { //sets activeModules to the current button box sutup
  activeModules.clear();

  for (int i = 0; i < allModules.size(); i++) {
    if (allModules[i]->initialize()) {
      activeModules.push_back(i);
    }
  }
}
