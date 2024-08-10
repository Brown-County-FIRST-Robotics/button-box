// #include <PicoGamepad.h>
#include <vector>

#include "i2c_boards.h"

std::array<I2cBoard, 1> allModules = {
  MCP23017("Oli Industrial Box", 0x20, {1, 2, 3, 4, 5, 6})
};

std::vector<int> activeModules = {}; //list of indices of allModules that are active

//TODO: check which modules are connected
void setup() {
  Serial.begin(9600);

  refreshModules();
}

void loop() {
  for (int i : activeModules) {
    allModules[i].update();
  }
}

void refreshModules() { //refreshes activeModules to the current button box sutup
  //run through allModules i2c ids to check which ones respond
  //initialize the new responsive ids
  //add the new indices to activeModules

  for (int i = 0; i < allModules.size(); i++) { //TEMP UNTIL DETECTION SYSTEM IS WORKING
    activeModules.push_back(i);
  }
  for (int i : activeModules) {
    allModules[i].initialize();
  }
}
