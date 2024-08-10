#include "i2c_boards.h"

void I2cBoard::initialize() {}
void I2cBoard::update() {}

MCP23017::MCP23017(String name, int id, std::array<int, 16> buttonBindings) {
    I2cBoard::name = name;
    I2cBoard::i2c_id = id;
    pinButtonBindings = buttonBindings;
}

void MCP23017::initialize() {
    if (!board.begin_I2C()) {
        Serial.println("Error.");
        while (1);
    }

    for (int i = 0; i < 16; i++) {
        if (pinButtonBindings[i] != 0) {
            board.pinMode(i, INPUT_PULLUP);
        }
    }
}

void MCP23017::update() {
    for (int i = 0; i < 16; i++) {
        if (pinButtonBindings[i] != 0) {
            if (!board.digitalRead(i) && !button_states[i] && debounce_timers[i] == 0) {
                Serial.println(std::to_string(pinButtonBindings[i-1]).c_str()); //activate button in DInput

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

ADS1015::ADS1015(String name, int id, std::array<int, 4> axisBindings) {
    I2cBoard::name = name;
    I2cBoard::i2c_id = id;
    pinAxisBindings = axisBindings;
}

void ADS1015::initialize() {

}

void ADS1015::update() {

}

ADS7830::ADS7830(String name, int id, std::array<int, 8> axisBindings) {
    I2cBoard::name = name;
    I2cBoard::i2c_id = id;
    pinAxisBindings = axisBindings;
}

void ADS7830::initialize() {

}

void ADS7830::update() {

}
