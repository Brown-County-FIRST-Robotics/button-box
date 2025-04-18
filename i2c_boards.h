#include <array>
#include <string>
#include <Adafruit_MCP23X17.h>
#include <Adafruit_ADS1X15.h>
#include <Adafruit_ADS7830.h>
#include "Outputs.h"

class I2cBoard {
    public:
        int i2c_id;

        virtual bool initialize(); //returns whether the board is connected
        virtual void update();
};

class MCP23017 : public I2cBoard {
    public:
        MCP23017(int id, std::vector<Output*> buttonBindings);

        bool initialize() override;
        void update() override;

    private:
        Adafruit_MCP23X17 board;
        std::vector<Output*> pinButtonBindings; //which DInput button each pin (A0-B7) is bound to

        bool button_states[16] = {};
        const int DEBOUNCE_TIME = 20;
        int debounce_timers[16] = {};
};
// Helper function to set axes based on number rather than name
void setJoystickAxis(int axis, int value);
