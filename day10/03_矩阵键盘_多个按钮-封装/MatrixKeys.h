#ifndef __MATRIX_KEYS_H__
#define __MATRIX_KEYS_H__

#include "Config.h"

void MK_init();

void MK_scan();

/**
 * 对于矩阵键盘的修改，由于涉及键位太多，懒得封装，具体去.c文件内GPIO_Config函数内修改吧
 * */

/*按键状态变化的回调函数，由外部实现，提供变化按键的行列信息*/
extern void MK_on_keydown(u8 row, u8 col);

extern void MK_on_keyup(u8 row, u8 col);

#endif
