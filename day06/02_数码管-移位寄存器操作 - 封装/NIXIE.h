#ifndef __NIXIE_H__
#define __NIXIE_H__

#include "Config.h"

#define NIX_DI P44  // 数据输入
#define NIX_SCK P42 // 移位寄存器
#define NIX_RCK P43 // 锁存寄存器

// 数码管控制IO初始化-初始化为推挽
#define GPIO_CONFIG()                                                          \
  P4M0 |= 0x1c;                                                                \
  P4M1 &= ~0x1c;

// 获取指定位的值
#define GET_BIT_VALUE(byte, pos) (byte & (1 << pos))

// 数码管初始化函数
void nixie_init();

/**
 * @note 数码管显示函数
 * @parm letter 字母位(控制要显示什么内容)
 * @parm digital 数字位(控制哪个数码管显示内容)
 */
void nixie_show(u8 letter, u8 digital);

/**
 *@note
 *字符显示函数，用户可以指定字符对应的索引，函数通过查表的形式显示指定的字符
 * @parm index 字符索引(具体需要去看源码数组或看笔记的表)
 * @parm digital 数字位(控制哪个数码管显示内容)
 */
void nixie_display(u8 index, u8 dig);

#endif
