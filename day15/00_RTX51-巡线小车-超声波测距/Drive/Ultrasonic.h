#ifndef __Ultrasonic_H__
#define __Ultrasonic_H__

#include "../Lib/Config.h"
#include "../Lib/GPIO.h"

// ECHO管脚初始化为高阻输入--P40
#define ECHO_IO_INIT() P4_MODE_IN_HIZ(GPIO_Pin_0)
// TRIG管脚初始化为推挽输出--P13
#define TRIG_IO_INIT() P1_MODE_OUT_PP(GPIO_Pin_3)

#define ECHO P40
#define TRIG P13

void Ultrasonic_init();
char Ultrasonic_get_distance(float *distance); // 以cm为单位

#endif // !__Ultrasonic_H__