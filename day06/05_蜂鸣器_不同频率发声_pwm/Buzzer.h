#ifndef __BUZZER_H
#define __BUZZER_H

#include "Config.h"

/*蜂鸣器引脚及GPIO*/
#define BUZZER_PIN GPIO_Pin_0
#define BUZZER_GPIO GPIO_P0


// 初始化蜂鸣器
void Buzzer_init();

// 按照指定频率播放
void Buzzer_play(u16 hz_val);

// 停止播放
void Buzzer_stop();

// 根据传入音阶播放音乐
void Buzzer_beep(u8 idx);

#endif
