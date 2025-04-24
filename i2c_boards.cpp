#include <cmath>
#include <Joystick.h>
#include "i2c_boards.h"

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
