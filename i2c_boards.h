#include <array>
#include <string>
#include <Adafruit_MCP23X17.h>
#include <Adafruit_ADS1X15.h>
#include <Adafruit_ADS7830.h>

// IMPORTANT NOTE: Dinput button and axis ids are from 1-128 and 1-8 respectively in this program so that 0 can be used as null (pin has no binding)

class I2cBoard {
    public:
        String name;
        int i2c_id;

        virtual bool initialize();
        virtual void update();
};

class MCP23017 : public I2cBoard {
    public:
        MCP23017(String name, int id, std::array<int, 16> buttonBindings);

        bool initialize();
        void update();

    private:
        Adafruit_MCP23X17 board;
        std::array<int, 16> pinButtonBindings; //which DInput button each pin (A0-B7) is bound to //TODO: check whether this refence will work

        bool button_states[16] = {};
        const int DEBOUNCE_TIME = 50;
        int debounce_timers[16] = {};
};

class ADS1015 : public I2cBoard {
    public:
        ADS1015(String name, int id, std::array<int, 4> axisBindings, bool differential = false);

        bool initialize();
        void update();
    
    private:
        Adafruit_ADS1015 board;
        std::array<int, 4> pinAxisBindings; //which DInput axis each pin is bound to

        const float DEADZONE = 0.01;
        bool differential;
};

class ADS7830 : public I2cBoard {
    public:
        ADS7830(String name, int id, std::array<int, 8> axisBindings, bool differential = false);

        bool initialize();
        void update();

    private:
        Adafruit_ADS7830 board;
        std::array<int, 8> pinAxisBindings; //which DInput axis each pin is bound to

        const float DEADZONE = 0.01;
        bool differential;
};
