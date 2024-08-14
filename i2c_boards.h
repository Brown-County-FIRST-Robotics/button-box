#include <array>
#include <string>
#include <Adafruit_MCP23X17.h>
#include <Adafruit_ADS1X15.h>
#include <Adafruit_ADS7830.h>

class I2cBoard {
    public:
        int i2c_id;

        virtual bool initialize(); //returns whether the board is connected
        virtual void update();
};

class MCP23017 : public I2cBoard {
    public:
        MCP23017(int id, std::array<int, 16> buttonBindings);

        bool initialize() override;
        void update() override;

    private:
        Adafruit_MCP23X17 board;
        std::array<int, 16> pinButtonBindings; //which DInput button each pin (A0-B7) is bound to

        bool button_states[16] = {};
        const int DEBOUNCE_TIME = 20;
        int debounce_timers[16] = {};
};

class ADS1015 : public I2cBoard {
    public:
        ADS1015(int id, std::array<int, 4> axisBindings, bool differential = false);

        bool initialize() override;
        void update() override;
    
    private:
        Adafruit_ADS1015 board;
        std::array<int, 4> pinAxisBindings; //which DInput axis each pin is bound to

        double getScaledReading(int id); // scales the reading to -1 to 1

        const float DEADZONE = 0.08;
        bool differential;
};

class ADS7830 : public I2cBoard {
    public:
        ADS7830(int id, std::array<int, 8> axisBindings, bool differential = false);

        bool initialize() override;
        void update() override;

    private:
        Adafruit_ADS7830 board;
        std::array<int, 8> pinAxisBindings; //which DInput axis each pin is bound to

        double getScaledReading(int id); // scales the reading to -1 to 1

        const float DEADZONE = 0.1;
        bool differential;
};
