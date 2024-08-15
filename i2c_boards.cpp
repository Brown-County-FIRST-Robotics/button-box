#include <cmath>
#include "i2c_boards.h"

bool I2cBoard::initialize() {return false;}
void I2cBoard::update() {}

MCP23017::MCP23017(int id, std::array<int, 16> buttonBindings) {
    I2cBoard::i2c_id = id;
    pinButtonBindings = buttonBindings;
}

bool MCP23017::initialize() {
    if (!board.begin_I2C(I2cBoard::i2c_id)) {
        return false;
    }

    for (int i = 0; i < 16; i++) {
        if (pinButtonBindings[i] != 0) {
            board.pinMode(i, INPUT_PULLUP);
        }
    }
    
    return true;
}

void MCP23017::update() {
    for (int i = 0; i < 16; i++) {
        if (pinButtonBindings[i] != 0) {
            if (!board.digitalRead(i) && !button_states[i] && debounce_timers[i] == 0) {
                dInput.pressButton(pinButtonBindings[i] - 1);

                button_states[i] = true;
                debounce_timers[i] = DEBOUNCE_TIME;
            }
            
            else {
                if (button_states[i] && board.digitalRead(i)) {
                  dInput.releaseButton(pinButtonBindings[i] - 1);
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

    for (int id : pinAxisBindings) {
      if (id != 0) {
        setDInputAxisRange(id - 1, 0, 1100);
      }
    }

    return true;
}

void ADS1015::update() {
  if (differential) { //Untested
    for (int i = 0; i < 2; i++) {
        if (pinAxisBindings[i] != 0) {
            int value = (i == 0 ? board.readADC_Differential_0_1() : board.readADC_Differential_2_3());
            if (std::abs(value) > DEADZONE) {
              setDInputAxis(pinAxisBindings[i] - 1, value);
            }
        }
    }
  }
  else {
    for (int i = 0; i < 4; i++) {
        if (pinAxisBindings[i] != 0) {
            if (std::abs(board.readADC_SingleEnded(i)) > DEADZONE) {
              setDInputAxis(pinAxisBindings[i] - 1, board.readADC_SingleEnded(i));
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
    
    for (int id : pinAxisBindings) {
      if (id != 0) {
        setDInputAxisRange(id - 1, 0, 255);
      }
    }

    return true;
}

void ADS7830::update() {
  if (differential) { //Untested
    for (int i = 0; i < 4; i++) {
      if (pinAxisBindings[i] != 0) {
        if (std::abs(board.readADCdifferential(i)) > DEADZONE) {
          setDInputAxis(pinAxisBindings[i] - 1, board.readADCdifferential(i));
        }
      }
    }
  }
  else {
    for (int i = 0; i < 8; i++) {
      if (pinAxisBindings[i] != 0) {
        if (std::abs(board.readADCsingle(i)) > DEADZONE) {
          setDInputAxis(pinAxisBindings[i] - 1, board.readADCsingle(i));
        }
      }
    }
  }
}

//Helper functions
void setDInputAxisRange(int axis, int32_t min, int32_t max) {
  switch (axis) {
    case 0:
      dInput.setXAxisRange(min, max);
      break;
    case 1:
      dInput.setYAxisRange(min, max);
      break;
    case 2:
      dInput.setZAxisRange(min, max);
      break;
    case 3:
      dInput.setRxAxisRange(min, max);
      break;
    case 4:
      dInput.setRyAxisRange(min, max);
      break;
    case 5:
      dInput.setRzAxisRange(min, max);
      break;
    case 6:
      dInput.setRudderRange(min, max);
      break;
    case 7:
      dInput.setThrottleRange(min, max);
      break;
    case 8:
      dInput.setAcceleratorRange(min, max);
      break;
    case 9:
      dInput.setBrakeRange(min, max);
      break;
    case 10:
      dInput.setSteeringRange(min, max);
      break;
  }
}

void setDInputAxis(int axis, int32_t value) {
  switch (axis) {
    case 0:
      dInput.setXAxis(value);
      break;
    case 1:
      dInput.setYAxis(value);
      break;
    case 2:
      dInput.setZAxis(value);
      break;
    case 3:
      dInput.setRxAxis(value);
      break;
    case 4:
      dInput.setRyAxis(value);
      break;
    case 5:
      dInput.setRzAxis(value);
      break;
    case 6:
      dInput.setRudder(value);
      break;
    case 7:
      dInput.setThrottle(value);
      break;
    case 8:
      dInput.setAccelerator(value);
      break;
    case 9:
      dInput.setBrake(value);
      break;
    case 10:
      dInput.setSteering(value);
      break;
  }
}
