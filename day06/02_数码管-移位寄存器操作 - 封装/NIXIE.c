#include "NIXIE.h"

// 锁存操作-多行宏
#define RCK_ACTION()                                                           \
  NIX_SCK = 0;                                                                 \
  NOP2();                                                                      \
  NIX_SCK = 1;                                                                 \
  NOP2();

void nixie_init() {
  // 初始化IO
  GPIO_CONFIG();
}

static void nixie_out(u8 dat) {
  char i;
  for (i = 7; i >= 0; i--) { //(二进制位是从右往左写的)
    // 获取预先定义的位数据，输出到芯片进行控制
    NIX_DI = GET_BIT_VALUE(dat, i);
    // 寄存器的移位操作
    NIX_RCK = 0;
    NOP2();
    NIX_RCK = 1;
    NOP2();
  }
}

void nixie_show(u8 letter, u8 digital) {

  // 1.先发送字母位(控制显示内容,至高位7是控制数码管右侧点的)
  // 1 111 1000 --> 7
  nixie_out(letter);

  // 2.再发送数字位(控制哪个位置点亮)
  // 1111 1011 -> 7.7.空 7. 7.7.7.7 (二进制位是从右往左写的)
  nixie_out(digital);

  // 3.锁存操作(制造上升沿)
  RCK_ACTION();
}

// Display函数通过查索引，显示指定的字符
// 索引对应表格参见：https://www.yuque.com/icheima/stc8h/kmz2mllvxs1uvdfy#lLhhp
u8 code LED_TABLE[] = {
    // 0 	1	 2	-> 9	(索引012...9)
    0xC0, 0xF9, 0xA4, 0xB0, 0x99, 0x92, 0x82, 0xF8, 0x80, 0x90,
    // 0. 1. 2. -> 9.	(索引10,11,12....19)
    0x40, 0x79, 0x24, 0x30, 0x19, 0x12, 0x02, 0x78, 0x00, 0x10,
    // . -						(索引20,21)
    0x7F, 0xBF,
    // AbCdEFHJLPqU		(索引22,23,24....33)
    0x88, 0x83, 0xC6, 0xA1, 0x86, 0x8E, 0x89, 0xF1, 0xC7, 0x8C, 0x98, 0xC1};

void nixie_display(u8 index, u8 dig) {
  // 查表，获取用户传入字符所对应的显示二进制串
  u8 letter = LED_TABLE[index];
  // 根据传入要显示的位置，移位，将对应位置置1
  u8 digital = (1 << dig);

  // 调用show函数显示字符
  nixie_show(letter, digital);
}
