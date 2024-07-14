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
  // os_create_task(2); // 创建巡线任务

  // 销毁自己
  os_delete_task(0);
}

u8 mode = 0; // 模式 0:前进 1:后退 2:停止
void KEY_down() {
  Light_on(ALL);

  // 按键按下，调整模式
  switch (mode) {
    case 0:
      Motor_forward(70);
      break;
    case 1:
      Motor_backward(30);
      break;
    case 2:
      Motor_left(70);
      break;
    case 3:
      Motor_right(70);
    default:
      Motor_stop();
      break;
  }
  mode++;
  if (mode > 4)
  {
    mode = 0;
  }
}

void KEY_up() { Light_off(ALL); }

void task_Key() _task_ 1 {

  while (1) {
    KEY_scan();
    os_wait2(K_TMO, 2);
  }
}

void task_Track() _task_ 2 {

  while (1) {
    u8 code states[5];
    char i;
    int pos;

    Track_get_state(&states);
    for (i = 4; i >= 0; i--) {
      printf("%d ", (int)states[i]);
    }
    printf("\n");

    pos = Track_get_position();
    printf("pos: %d\n", pos);

    os_wait2(K_TMO, 40); // 5 * 40 = 200ms,每200ms获取一次
  }
}