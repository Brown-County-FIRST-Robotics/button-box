#include <Adafruit_MCP23X17.h>
// #include <PicoGamepad.h>

Adafruit_MCP23X17 device;

#define DISCHARGE_ID 0
#define CHAINS_ID 1
#define CONVEYER_ID 2
#define JOG_CHAINS_F_ID 3
#define JOG_CHAINS_R_ID 4

bool button_states[5] = {false};
#define DEBOUNCE_TIME 50;
int debounce_timers[5] = {0};

void setup() {
  Serial.begin(9600);

  if (!device.begin_I2C()) {
    Serial.println("Error.");
    while (1);
  }
  device.pinMode(DISCHARGE_ID, INPUT_PULLUP);
  device.pinMode(CHAINS_ID, INPUT_PULLUP);
  device.pinMode(CONVEYER_ID, INPUT_PULLUP);
  device.pinMode(JOG_CHAINS_R_ID, INPUT_PULLUP);
  device.pinMode(JOG_CHAINS_F_ID, INPUT_PULLUP);
}

void loop() {
  for (int i = 0; i < 5; i++) {
    if (!device.digitalRead(i) && !button_states[i] && debounce_timers[i] == 0) {
      switch (i) {
        case DISCHARGE_ID:
          Serial.println("Discharge");
          break;
        case CHAINS_ID:
          Serial.println("Chains");
          break;
        case CONVEYER_ID:
          Serial.println("Conveyer");
          break;
        case JOG_CHAINS_R_ID:
          Serial.println("Jog Chains Reverse");
          break;
        case JOG_CHAINS_F_ID:
          Serial.println("Jog Chains Forward");
          break;
      }
      button_states[i] = true;
      debounce_timers[i] = DEBOUNCE_TIME;
    }

    if (device.digitalRead(i) && button_states[i]) {
      button_states[i] = false;
    }

    if (debounce_timers[i] > 0) {
      debounce_timers[i]--;
    }
  }
}