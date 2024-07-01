#include "Config.h"
#include "GPIO.h"

// 数据信号
#define NIX_DI P44
// 移位信号
#define NIX_RCK P43
// 锁存信号
#define NIX_SCK P42

void gpio_config() {
  // 初始化数码管控制引脚
  P4_MODE_OUT_PP(GPIO_P2 | GPIO_P3 | GPIO_P4);
}

// 记录字母位(控制要显示什么内容)
u8 letter = 0x02; // 数字6 -> 0000 0010
// 记录数字位(控制哪个数码管显示内容)
u8 digital = 0x0A; // 0000 1010(左侧两个数码管点亮)

#define GET_BIT_VALUE(byte, pos) (byte & (1 << pos))

int main(void) {
  char i;
  gpio_config();

  // 1.先发送字母位(控制显示内容,至高位7是控制数码管右侧点的)
  // 1 111 1000 --> 7
  for (i = 7; i >= 0; i--) { //(二进制位是从右往左写的)
    // 获取预先定义的位数据，输出到芯片进行控制
    NIX_DI = GET_BIT_VALUE(letter, i);
    // 寄存器的移位操作
    NIX_RCK = 0;
    NOP2();
    NIX_RCK = 1;
    NOP2();
  }

  // 2.再发送数字位(控制哪个位置点亮)
  // 1111 1011 -> 7.7.空 7. 7.7.7.7 (二进制位是从右往左写的)
  for (i = 7; i >= 0; i--) {
    // 获取预先定义的位数据，输出到芯片进行控制
    NIX_DI = GET_BIT_VALUE(digital, i);
    // 寄存器的移位操作
    NIX_RCK = 0;
    NOP2();
    NIX_RCK = 1;
    NOP2();
  }

  // 3.锁存操作(制造上升沿)
  NIX_SCK = 0;
  NOP2();
  NIX_SCK = 1;
  NOP2();

  while (1) {
  }
}