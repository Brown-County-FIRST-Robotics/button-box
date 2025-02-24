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

ADS1015::ADS1015(int id, std::array<int, 4> axisBindings, bool differential) {
    I2cBoard::i2c_id = id;
    pinAxisBindings = axisBindings;
    this->differential = differential;
}

bool ADS1015::initialize() {
    if (!board.begin(I2cBoard::i2c_id)) {
        return false;
    }

    return true;
}

void ADS1015::update() {
  if (differential) { //Untested
    for (int i = 0; i < 2; i++) {
        if (pinAxisBindings[i] != 0) {
            int value = (i == 0 ? board.readADC_Differential_0_1() : board.readADC_Differential_2_3());
            if (std::abs(value) > DEADZONE) {
              Joystick.use10bit();
              setJoystickAxis(pinAxisBindings[i] - 1, value);
            }
        }
    }
  }
  else {
    for (int i = 0; i < 4; i++) {
        if (pinAxisBindings[i] != 0) {
            if (std::abs(board.readADC_SingleEnded(i)) > DEADZONE) {
              Joystick.use10bit();
              setJoystickAxis(pinAxisBindings[i] - 1, board.readADC_SingleEnded(i));
            }
        }
    }
  }
}

ADS7830::ADS7830(int id, std::array<int, 8> axisBindings, bool differential) {
    I2cBoard::i2c_id = id;
    pinAxisBindings = axisBindings;
    this->differential = differential;
}

bool ADS7830::initialize() {
    if (!board.begin(I2cBoard::i2c_id)) {
        return false;
    }
    
    return true;
}

void ADS7830::update() {
  if (differential) { //Untested //TODO: simplify the analog update functions to merge differential and singlended more
    for (int i = 0; i < 4; i++) {
      if (pinAxisBindings[i] != 0) {
        if (std::abs(board.readADCdifferential(i)) > DEADZONE) {
          Joystick.use8bit(false); //TODO: check what the parameter in this does (I think it changes whether it's -127 to 127 or 0 to 127)
          setJoystickAxis(pinAxisBindings[i] - 1, board.readADCdifferential(i));
        }
      }
    }
  }
  else {
    for (int i = 0; i < 8; i++) {
      if (pinAxisBindings[i] != 0) {
        if (std::abs(board.readADCsingle(i)) > DEADZONE) {
          Joystick.use8bit(false);
          setJoystickAxis(pinAxisBindings[i] - 1, board.readADCsingle(i));
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
