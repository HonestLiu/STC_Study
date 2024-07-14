#include "Light.h"

void Light_init(void) { GPIO_INIT(); }

// 开灯
void Light_on(Light light) {
  switch (light) {
  case LEFT:
    L_LIGHT = 1;
    break;
  case RIGHT:
    R_LIGHT = 1;
    break;
  case ALL:
    L_LIGHT = 1;
    R_LIGHT = 1;
    break;
  }
}

// 关灯
void Light_off(Light light) {
  switch (light) {
  case LEFT:
    L_LIGHT = 0;
    break;
  case RIGHT:
    R_LIGHT = 0;
    break;
  case ALL:
    L_LIGHT = 0;
    R_LIGHT = 0;
    break;
  }
}