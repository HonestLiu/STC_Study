#ifndef __Light_H_
#define __Light_H_

#include "Lib/GPIO.h"

#define GPIO_INIT()                                                            \
  P0_MODE_OUT_PP(GPIO_Pin_5);                                                  \
  P3_MODE_OUT_PP(GPIO_Pin_4);
#define L_LIGHT P34
#define R_LIGHT P05


typedef enum {
  LEFT,
  RIGHT,
  ALL,
} Light;

void Light_init(void);
void Light_on(Light light);
void Light_off(Light light);

#endif // !__Light_H_
