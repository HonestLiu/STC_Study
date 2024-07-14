// 外部设备
#include "../Drive/Buzzer.h"
#include "../Drive/Key.h"
#include "../Drive/Light.h"
#include "../Drive/Motors.h"
#include "../Drive/Track.h"
#include "../Drive/Ultrasonic.h"
#include "../Drive/battery.h"

// 片上外设
#include "../Lib/Config.h"
#include "../Lib/NVIC.h"
#include "../Lib/Switch.h"
#include "../Lib/UART.h"

#include "RTX51TNY.h" //RTX51的系统头文件
#include <cstdio>
#include <stdio.h>

void UART_config(void) {
  // >>> 记得添加 NVIC.c, UART.c, UART_Isr.c <<<
  COMx_InitDefine COMx_InitStructure; // 结构定义
  COMx_InitStructure.UART_Mode =
      UART_8bit_BRTx; // 模式,
                      // UART_ShiftRight,UART_8bit_BRTx,UART_9bit,UART_9bit_BRTx
  COMx_InitStructure.UART_BRT_Use =
      BRT_Timer1; // 选择波特率发生器, BRT_Timer1, BRT_Timer2 (注意:
                  // 串口2固定使用BRT_Timer2)
  COMx_InitStructure.UART_BaudRate = 115200ul; // 波特率, 一般 110 ~ 115200
  COMx_InitStructure.UART_RxEnable = ENABLE; // 接收允许,   ENABLE或DISABLE
  COMx_InitStructure.BaudRateDouble = DISABLE; // 波特率加倍, ENABLE或DISABLE
  UART_Configuration(
      UART1, &COMx_InitStructure); // 初始化串口1 UART1,UART2,UART3,UART4

  NVIC_UART1_Init(ENABLE,
                  Priority_1); // 中断使能, ENABLE/DISABLE; 优先级(低到高)
                               // Priority_0,Priority_1,Priority_2,Priority_3
  UART1_SW(
      UART1_SW_P30_P31); // 引脚选择,
                         // UART1_SW_P30_P31,UART1_SW_P36_P37,UART1_SW_P16_P17,UART1_SW_P43_P44
}

void sys_init() {
  EAXSFR();
  // 片上外设初始化
  UART_config(); // 初始化UART

  // 驱动初始化
  Light_init();      // 初始化转向灯
  KEY_init();        // 初始化按钮
  Battery_init();    // 初始化ADC电源检测
  buzzer_init();     // 初始化蜂鸣器
  Ultrasonic_init(); // 初始化超声波模块
  Track_init();      // 初始化巡线模块
  Motor_init();      // 初始化电机控制

  EA = 1;
}

void start_main() _task_ 0 {
  sys_init();
  os_create_task(1); // 创建按键任务

  // 销毁自己
  os_delete_task(0);
}

u8 track_state = 0; // 巡线功能状态 0：未启用 1:已启用
void KEY_down() {
  Light_on(ALL);
  if (!track_state) {
    printf("启用巡线\n");
    os_create_task(2); // 创建巡线任务
  } else {
    printf("关闭巡线\n");
    os_delete_task(2); // 销毁巡线任务
  }
  // 更新巡线状态
  track_state = !track_state;
}

void KEY_up() { Light_off(ALL); }

void task_Key() _task_ 1 {

  while (1) {
    KEY_scan();
    os_wait2(K_TMO, 2);
  }
}

void task_Track() _task_ 2 {
	int pos = 0, last_pos = -10;
  while (1) {
    

    pos = Track_get_position();
    //如果pos等于last_pos，则跳过当前调用，节约系统资源
    if (pos == last_pos) {
      os_wait2(K_TMO, 2);//休眠一小会，避免其重复调用Track_get_position
      continue;
    }
    //更新last_pos
    last_pos = pos;
		printf("pos: %d\n", pos);
		
    if (pos < 0) {
      printf("Motor_left\n");
      Motor_left(60);
    } else if (pos > 0) {
      printf("Motor_right\n");
      Motor_right(60);
    } else {
      printf("Motor_forward\n");
      Motor_forward(70);
    }

    os_wait2(K_TMO, 4); // 5 * 4 = 20ms,每20ms获取一次
  }
}