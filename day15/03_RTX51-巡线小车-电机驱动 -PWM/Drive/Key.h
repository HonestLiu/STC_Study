#ifndef __KEY_H__
#define __KEY_H__

#include "../Lib/GPIO.h"
#include "../Lib/Config.h"

// IO初始化
#define KEY P37
#define KEY_IO_INIT() P3_MODE_IO_PU(GPIO_Pin_7)

// 按键初始化
void KEY_init(void);
// 按键监听
void KEY_scan(void);

// 事件回调函数,由使用者外部实现
extern void KEY_down();
extern void KEY_up();

#endif // !__KEY_H__