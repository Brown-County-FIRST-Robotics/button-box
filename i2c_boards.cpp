#include <cmath>
#include "i2c_boards.h"

bool I2cBoard::initialize() {return false;}
void I2cBoard::update() {}

MCP23017::MCP23017(String name, int id, std::array<int, 16> buttonBindings) {
    I2cBoard::name = name;
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
            if (!board.digitalRead(i) && !button_states[i] && debounce_timers[i] == 0) { //TODO: try using board.readGPIOAB() rather than digitalRead
                Serial.println(String(pinButtonBindings[i] - 1)); //activate button in DInput //NOTE: String may be too large for embedded

                button_states[i] = true;
                debounce_timers[i] = DEBOUNCE_TIME;
            }
            
            else {
                if (button_states[i] && board.digitalRead(i)) {
                    button_states[i] = false;
                }
                if (debounce_timers[i] > 0) {
                    debounce_timers[i]--;
                }
            }
        }
    }
}

ADS1015::ADS1015(String name, int id, std::array<int, 4> axisBindings, bool differential) {
    I2cBoard::name = name;
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
  if (differential) {
    for (int i = 0; i < 2; i++) {
        if (pinAxisBindings[i] != 0) {
            int16_t value = (i == 0 ? board.readADC_Differential_0_1() : board.readADC_Differential_2_3());
            if (std::abs(value) > DEADZONE) {
                Serial.println("Axis " +  String(pinAxisBindings[i] - 1) + ": " + String(value)); //Update DInput axis
                //TODO: add cooldown to not saturate println
            }
        }
    }
  }
  else {
    for (int i = 0; i < 4; i++) {
        if (pinAxisBindings[i] != 0) {
            if (std::abs(board.readADC_SingleEnded(i)) > DEADZONE) {
                Serial.println("Axis " +  String(pinAxisBindings[i] - 1) + ": " + String(board.readADC_SingleEnded(i))); //Update DInput axis
                //TODO: add cooldown to not saturate println
            }
        }
    }
  }
}

ADS7830::ADS7830(String name, int id, std::array<int, 8> axisBindings, bool differential) {
    I2cBoard::name = name;
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
  if (differential) {
    for (int i = 0; i < 4; i++) {
        if (pinAxisBindings[i] != 0) {
            if (std::abs(board.readADCdifferential(i)) > DEADZONE) {
                Serial.println("Axis " +  String(pinAxisBindings[i] - 1) + ": " + String(board.readADCdifferential(i))); //Update DInput axis
                //TODO: add cooldown to not saturate println
            }
        }
    }
  }
  else {
    for (int i = 0; i < 8; i++) {
        if (pinAxisBindings[i] != 0) {
            if (std::abs(board.readADCsingle(i)) > DEADZONE) {
                Serial.println("Axis " +  String(pinAxisBindings[i] - 1) + ": " + String(board.readADCsingle(i))); //Update DInput axis
                //TODO: add cooldown to not saturate println
            }
        }
    }
  }
}
