#include <array>
#include <string>
#include <Adafruit_MCP23X17.h>
#include <Adafruit_ADS1X15.h>
#include <Adafruit_ADS7830.h>
#include "Outputs.h"

// Helper function to set axes based on number rather than name
void setJoystickAxis(int axis, int value);

static std::map<int, Adafruit_MCP23X17*> boardz{};  // Map between indicies and real i2c boards

static Adafruit_MCP23X17* getBoard(int ind) {
  if (boardz.count(ind) == 0) {
    boardz[ind] = new Adafruit_MCP23X17();
  }
  return boardz[ind];
}