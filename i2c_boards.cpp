#include <cmath>
#include <Joystick.h>

#include "i2c_boards.h"

bool I2cBoard::initialize() {return false;}
void I2cBoard::update() {}

MCP23017::MCP23017(int id, std::vector<Output*> buttonBindings) {
    I2cBoard::i2c_id = id;
    pinButtonBindings = buttonBindings;
}

bool MCP23017::initialize() {
    if (!board.begin_I2C(I2cBoard::i2c_id)) {
        return false;
    }

    for (int i = 0; i < pinButtonBindings.size(); i++) {
        if (pinButtonBindings[i] != 0) {
            board.pinMode(i, INPUT_PULLUP);
        }
    }
    
    return true;
}

void MCP23017::update() {
    for (int i = 0; i < pinButtonBindings.size(); i++) {
        if (pinButtonBindings[i] != 0) {
            if (!board.digitalRead(i) && !button_states[i] && debounce_timers[i] == 0) {
//              Serial.println("change");
                pinButtonBindings[i]->Set(true);

                button_states[i] = true;
                debounce_timers[i] = DEBOUNCE_TIME;
            }
            
            else {
                if (button_states[i] && board.digitalRead(i)) {
//                                Serial.println("change");

                    pinButtonBindings[i]->Set(false);
                  button_states[i] = false;
                }
                if (debounce_timers[i] > 0) {
                    debounce_timers[i]--;
                }
            }
        }
    }
}


// Helper function to set axes based on number rather than name
void setJoystickAxis(int axis, int value) {
  switch (axis) {
    case 0:
      Joystick.X(value);
      break;
    case 1:
      Joystick.Y(value);
      break;
    case 2:
      Joystick.Z(value);
      break;
    case 3:
      Joystick.Zrotate(value);
      break;
    case 4:
      Joystick.sliderLeft(value);
      break;
    case 5:
      Joystick.sliderRight(value);
      break;
  }
}
