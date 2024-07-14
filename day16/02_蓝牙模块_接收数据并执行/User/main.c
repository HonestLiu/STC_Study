// 外部设备
#include "../Drive/Key.h"
#include "../Drive/Light.h"
#include "../Drive/Motors.h"
#include "../Drive/Track.h"
#include "../Drive/Ultrasonic.h"
#include "../Drive/battery.h"
#include "../Drive/Buzzer.h"

// 片上外设
#include "../Lib/Config.h"
#include "../Lib/GPIO.h"
#include "../Lib/NVIC.h"
#include "../Lib/Switch.h"
#include "../Lib/UART.h"

#include "RTX51TNY.h" //RTX51的系统头文件

static void GPIO_config() {
  GPIO_InitTypeDef gpio_init_struct;
  gpio_init_struct.Mode = GPIO_PullUp;
  gpio_init_struct.Pin = GPIO_Pin_1 | GPIO_Pin_0;
  GPIO_Inilize(GPIO_P1, &gpio_init_struct);
}

void UART_config(void) {

  /* UART1初始化 —— PC to MCU */
  COMx_InitDefine COMx_InitStructure;
  COMx_InitStructure.UART_Mode = UART_8bit_BRTx;
  COMx_InitStructure.UART_BRT_Use = BRT_Timer1;
  COMx_InitStructure.UART_BaudRate = 115200ul;
  COMx_InitStructure.UART_RxEnable = ENABLE;
  COMx_InitStructure.BaudRateDouble = DISABLE;
  UART_Configuration(UART1, &COMx_InitStructure);
  NVIC_UART1_Init(ENABLE, Priority_1);
  UART1_SW(UART1_SW_P30_P31);

  /* UART2初始化 —— MCU to BlueTooth */
  COMx_InitStructure.UART_Mode = UART_8bit_BRTx;
  COMx_InitStructure.UART_BRT_Use = BRT_Timer2;
  COMx_InitStructure.UART_BaudRate = 9600ul; // 波特率, 蓝牙模块默认就是9600?
  COMx_InitStructure.UART_RxEnable = ENABLE;
  COMx_InitStructure.BaudRateDouble = DISABLE;
  UART_Configuration(UART2, &COMx_InitStructure);
  NVIC_UART2_Init(ENABLE, Priority_1);
  UART2_SW(UART2_SW_P10_P11);
}

void sys_init() {
  EAXSFR();
  // 片上外设初始化
  UART_config(); // 初始化UART
  GPIO_config();

  // 驱动初始化
  Light_init(); // 初始化转向灯
  KEY_init();   // 初始化按钮
  Battery_init();    // 初始化ADC电源检测
  buzzer_init();     // 初始化蜂鸣器
  Ultrasonic_init(); // 初始化超声波模块
  Track_init();      // 初始化巡线模块
  Motor_init();      // 初始化电机控制

  EA = 1;
}

#define UART1_TASK 2
#define UART2_TASK 3

void start_main() _task_ 0 {
  sys_init();
  os_create_task(1); // 创建按键任务
  os_create_task(UART1_TASK);
  os_create_task(UART2_TASK);

  // 销毁自己
  os_delete_task(0);
}

u8 track_state = 0; // 巡线功能状态 0：未启用 1:已启用
void KEY_down() { Light_on(ALL); }

void KEY_up() { Light_off(ALL); }

void task_Key() _task_ 1 {

  while (1) {
    KEY_scan();
    os_wait2(K_TMO, 2);
  }
}

// 从PC端接收数据，并将数据发送给蓝牙模块
void task_uart1() _task_ UART1_TASK {
  u8 i;
  while (1) {
    if (COM1.RX_TimeOut > 0) {
      // 超时计数
      if (--COM1.RX_TimeOut == 0) {
        if (COM1.RX_Cnt > 0) {
          // 这里处理收到的数据，做具体的逻辑，可以调用自己的on_uart1_recv
          for (i = 0; i < COM1.RX_Cnt; i++) {
            // 写出数据到蓝牙
            TX2_write2buff(RX1_Buffer[i]);
            TX1_write2buff(RX1_Buffer[i]);
          }
        }
        COM1.RX_Cnt = 0;
      }
    }
    os_wait2(K_TMO, 2);
  }
}

void bt_controller() {
  u8 dat = RX2_Buffer[0];
  
  if (dat == 0x01) {//前进
    Motor_forward(70);
  }else if (dat == 0x02)//后退
  {
    Motor_backward(70);
  }else if (dat == 0x03)
  { 
    Motor_stop();
  }else if (dat == 0x04)//开灯
  {
    Light_on(ALL);
    buzzer_beep();
  }else if (dat == 0x05)//关灯
  {
    Light_off(ALL);
    buzzer_beep();
  }
}

// 接收蓝牙模块发送回来的数据，并将数据发送给PC
void task_uart2() _task_ UART2_TASK {
  u8 i;
  while (1) {
    if (COM2.RX_TimeOut > 0) {
      // 超时计数
      if (--COM2.RX_TimeOut == 0) {
        if (COM2.RX_Cnt > 0) {
          bt_controller();
          for (i = 0; i < COM2.RX_Cnt; i++) {
            // 写出数据到PC
            TX1_write2buff(RX2_Buffer[i]);
          }
        }
        COM2.RX_Cnt = 0;
      }
    }
    os_wait2(K_TMO, 2);
  }
}
