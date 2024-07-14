#ifndef __BUZZER_H_
#define __BUZZER_H_

#include "../Lib/Config.h"
#include "../Lib/GPIO.h"

#define BUZZER_IO_INIT() P3_MODE_OUT_PP(GPIO_Pin_6)
#define BUZZER P36

void buzzer_init();
// 无源蜂鸣器
void buzzer_beep();

// 有源蜂鸣器
void buzzer_open();
void buzzer_close();

#endif // !__BUZZER_H_
