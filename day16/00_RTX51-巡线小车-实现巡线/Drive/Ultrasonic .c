#include "Ultrasonic.h"

void Delay10us(void) //@24.000MHz
{
  unsigned char data i;

  i = 78;
  while (--i)
    ;
}

static void GPIO_config(void) {

    // 初始化两个IO
    // TRIG 推挽输出模式
    GPIO_InitTypeDef	GPIO_InitStructure;		//结构定义
    GPIO_InitStructure.Pin  = GPIO_Pin_3;		//指定要初始化的IO,
    GPIO_InitStructure.Mode = GPIO_OUT_PP;	//指定IO的输入或输出方式,GPIO_PullUp,GPIO_HighZ,GPIO_OUT_OD,GPIO_OUT_PP
    GPIO_Inilize(GPIO_P1, &GPIO_InitStructure);//初始化

    // ECHO 高阻输入
    GPIO_InitStructure.Pin  = GPIO_Pin_0;		//指定要初始化的IO,
    GPIO_InitStructure.Mode = GPIO_HighZ;	//指定IO的输入或输出方式,GPIO_PullUp,GPIO_HighZ,GPIO_OUT_OD,GPIO_OUT_PP
    GPIO_Inilize(GPIO_P4, &GPIO_InitStructure);//初始化
}

void Ultrasonic_init() {
  // 初始化超声波模块管脚
  /*  ECHO_IO_INIT();
   TRIG_IO_INIT(); */
  GPIO_config();
  // 设置TRIG管脚均为低电平，等待上升沿的发生
  TRIG = 0;
}

char Ultrasonic_get_distance(float *distance) {
  u16 cnt_10us = 0;
  float dis = 0;
  // 1.拉高TRIG管脚10us以上,可发出 8 个 40KHZ的超声波脉冲(为规避误差持续20us)
  TRIG = 1;
  Delay10us();
  Delay10us();
  TRIG = 0;
  // 2.等待400us，等ECHO被拉高，如果没有则返回-1
  cnt_10us = 0;
  while (cnt_10us < 40 && ECHO == 0) {
    Delay10us();
    cnt_10us++;
  }
  if (cnt_10us * 10 >= 400) {
    // 超时退出，直接返回-1
    return cnt_10us;
  }
  // 3.记录ECHO从上升沿到下降沿的时间
  cnt_10us = 0;
  while (ECHO == 1) {
    Delay10us();
    cnt_10us++;
  }
  // 4.根据公式将时间转换为距离 --> 距离值为：(高电平时间*34cm/ms)/2
  dis = 34 * cnt_10us * 0.01 * 0.5; // cm/ms
  *distance = dis;
  if (dis < 2) {
    // 距离过短
    return 1;
  } else if (dis > 600) {
    // 距离过长
    return 2;
  }
  return 0; // 数据正常
}