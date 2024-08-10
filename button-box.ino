// #include <PicoGamepad.h>

#include "i2c_boards.h"

const I2cBoard ALL_MODULES[1] = {
  MCP23017("Oli Industrial Box", 0x20, {1, 2, 3, 4, 5, 6})
};

//TODO: check which modules are connected
void setup() {
  Serial.begin(9600);
  for (I2cBoard b : ALL_MODULES) {
    b.initialize();
  }
}

void loop() {
  for (I2cBoard b : ALL_MODULES) {
    b.update();
  }
}