#ifndef __BATTERY_H__
#define __BATTERY_H__

#include "../Lib/Config.h"

void Battery_init();
void Battery_get_voltage(float *voltage);

#endif // !__BATTERY_H__
