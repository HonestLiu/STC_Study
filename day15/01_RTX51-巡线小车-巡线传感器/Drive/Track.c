#include "Track.h"
#include "../Lib/GPIO.h"

void Track_init() {
  // 初始化GPIO
  P4_MODE_IO_PU(GPIO_Pin_7);
  P0_MODE_IO_PU(GPIO_Pin_6);
  P3_MODE_IO_PU(GPIO_Pin_2 | GPIO_Pin_3);
  P5_MODE_IO_PU(GPIO_Pin_0);
}

void Track_get_state(u8 states[]) {
  // 获取各个管脚的状态然后填入数组
  states[0] = TRACK_0;
  states[1] = TRACK_1;
  states[2] = TRACK_2;
  states[3] = TRACK_3;
  states[4] = TRACK_4;
}

/*
 * 电平状态分析:
 * - 1:高电平，不亮，压到线
 * - 0:低电平，亮，没压到线
 * 坐标建立: 4个光电传感器，对应位置光电传感器检测到就偏移指定的角度，以实现转弯
 * -64 -32 0 32 64
 */
int Track_get_position() {
  int sum = 0, count = 0, pos = 0;

  static int last_pos = 0;

  // 逐个判断各个光电传感器的电平值，以确定要转弯的角度
  if (TRACK_0 == 1) {
    sum += 64;
    count++;
  }

  if (TRACK_1 == 1) {
    sum += 32;
    count++;
  }

  if (TRACK_2 == 1) {
    sum += 0;
    count++;
  }

  if (TRACK_3 == 1) {
    sum += -32;
    count++;
  }

  if (TRACK_4 == 1) {
    sum += 64;
    count++;
  }

  // 状态丢失，保持上一次的偏移角度
  if (count == 0) {
    return last_pos;
  }

  // 求转弯角度的均值
  pos = sum / count;

  // 把当前最新的值记录下来,用于下一次没压到线的时候,返回
  last_pos = pos;

  return pos;
}