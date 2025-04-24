#include <vector>
#include <algorithm>
#include <Joystick.h>

#include "i2c_boards.h"

Joystick_ dInput{};

struct Panel {
  int id;
  std::vector<int> binds;
};

std::vector<Panel> allModules = {
  { 0, { 1, 2, 0, 3, 5, 4, 7, 6 } },
  { 2, { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 } }
};

int indexToi2cID(int ind) {
  return ind + 32;
}

std::vector<int> activeModules;  //list of indices of allModules that are active

void setup() {
  Serial.begin(9600);

  for (int mod = 0; mod < allModules.size(); mod++) {
    if (getBoard(allModules[mod].id)->begin_I2C(indexToi2cID(allModules[mod].id))) {
      activeModules.push_back(mod);
      auto bd = getBoard(allModules[mod].id);
      for (int i = 0; i < allModules[mod].binds.size(); i++) {
        bd->pinMode(i, INPUT_PULLUP);
      }
    }
  }
  Serial.println(activeModules.size());
  std::sort(activeModules.begin(), activeModules.end(), [](int a, int b) {
    return allModules[a].id < allModules[b].id;
  });
  // joystick initialization
  Joystick.begin();
  Joystick.useManualSend(true);
}

void writePanels() {
  bool present[] = { false, false, false, false, false, false, false, false };
  for (int mod : activeModules) {
    present[allModules[mod].id] = true;
  }
  for (int i = 0; i < 8; i++) {
    writeOut(i + 1, present[i]);
  }
}

void loop() {
  long start = micros();
  writePanels();
  int curr_index = 9;
  for (auto pan : activeModules) {
    auto panel = allModules[pan];
    for (uint8_t pin = 0; pin < panel.binds.size(); pin++) {
      bool value = getBoard(panel.id)->digitalRead(pin);
      writeOut(curr_index + panel.binds[pin], !value);
    }
    curr_index += panel.binds.size();
  }
  writeCaches();
  Joystick.send_now();
  Serial.println(micros() - start);
  delay(22);
}
