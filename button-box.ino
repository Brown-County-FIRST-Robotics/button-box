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

//MutexCoordinater mtx({allocJ()}

//MutexBitOutput* mtxbo(){
//}

std::vector<Module> allModules = {
  {"Oli Industrial Box", {new MCP23017(0x20, {allocJ(0+2),allocJ(0+3),allocJ(0+1),allocJ(0+4),allocJ(0+6),allocJ(0+5),allocJ(8),allocJ(7)})}},
  {"Oli Industrial Box2", {new MCP23017(0x23, {allocJ(8+3),allocJ(8+12),allocJ(8+4),allocJ(8+13),allocJ(8+5),allocJ(8+8),allocJ(8+1),allocJ(8+6),allocJ(8+14),allocJ(8+10),allocJ(8+11),allocJ(8+7),allocJ(8+2),allocJ(8+9),allocJ(8+16),allocJ(8+15)})}},
  {"Panel3", {new MCP23017(0x22, {allocJ(1),allocJ(2),allocJ(3),allocJ(4),allocJ(5),allocJ(6),allocJ(7),allocJ(8),allocJ(9)})}}},
//{"Fancy Joystick", {new ADS7830(0x48, {})}},
//  {"Accurate Analog", {new ADS1015(0x49, {1, 2})}}
};

std::vector<int> activeModules; //list of indices of allModules that are active

void setup() {
//  Serial.begin(9600);

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

  

//   Serial.println("Active modules:");
//   for (int i : activeModules) {
//     Serial.println("asd");
//   }
}
