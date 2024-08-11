// #include <PicoGamepad.h>
#include <vector>

#include "i2c_boards.h"

std::array<I2cBoard, 1> allModules = {
  MCP23017("Oli Industrial Box", 0x20, {1, 2, 3, 4, 5, 6})
};

std::vector<int> activeModules; //list of indices of allModules that are active

//TODO: check which modules are connected
void setup() {
  Serial.begin(9600);

  refreshModules();
}

void loop() {
  for (int i : activeModules) {
    allModules[i].update(); //TODO: check what happens when an i2c device is disconnected and then the program attempts to read from it
  }
  //TODO?: Add button to refresh modules, or refresh periodically
}

void refreshModules() { //refreshes activeModules to the current button box sutup
  activeModules.clear();

  for (int i = 0; i < allModules.size(); i++) {
    if (allModules[i].initialize()) {
      activeModules.push_back(i); //TODO: take action to prevent reinitialization, either add a deactivate function or check current modules without initializing them
    }
  }
}
