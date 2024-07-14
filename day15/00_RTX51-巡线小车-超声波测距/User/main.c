#include "../Drive/Key.h"
#include "../Drive/Light.h"
#include "../Drive/battery.h"
#include "../Drive/Buzzer.h"
#include "../Drive/Ultrasonic.h"
#include "../Lib/Config.h"
#include "../Lib/UART.h"
#include "../Lib/NVIC.h"
#include "../Lib/Switch.h"
#include "RTX51TNY.h" //RTX51的系统头文件


void UART_config(void) {
	// >>> 记得添加 NVIC.c, UART.c, UART_Isr.c <<<
    COMx_InitDefine		COMx_InitStructure;					//结构定义
    COMx_InitStructure.UART_Mode      = UART_8bit_BRTx;	//模式, UART_ShiftRight,UART_8bit_BRTx,UART_9bit,UART_9bit_BRTx
    COMx_InitStructure.UART_BRT_Use   = BRT_Timer1;			//选择波特率发生器, BRT_Timer1, BRT_Timer2 (注意: 串口2固定使用BRT_Timer2)
    COMx_InitStructure.UART_BaudRate  = 115200ul;			//波特率, 一般 110 ~ 115200
    COMx_InitStructure.UART_RxEnable  = ENABLE;				//接收允许,   ENABLE或DISABLE
    COMx_InitStructure.BaudRateDouble = DISABLE;			//波特率加倍, ENABLE或DISABLE
    UART_Configuration(UART1, &COMx_InitStructure);		//初始化串口1 UART1,UART2,UART3,UART4

  	NVIC_UART1_Init(ENABLE,Priority_1);		//中断使能, ENABLE/DISABLE; 优先级(低到高) Priority_0,Priority_1,Priority_2,Priority_3
    UART1_SW(UART1_SW_P30_P31);		// 引脚选择, UART1_SW_P30_P31,UART1_SW_P36_P37,UART1_SW_P16_P17,UART1_SW_P43_P44
}

void sys_init() {
  EAXSFR();

  UART_config();//初始化UART
  Light_init();//初始化转向灯
  KEY_init();//初始化按钮
  Battery_init(); // 初始化ADC电源检测
  buzzer_init();  // 初始化蜂鸣器
  Ultrasonic_init();//初始化超声波模块

  EA = 1;
}

void start() _task_ 0 {
  sys_init();
  os_create_task(1); // 创建任务1
                     // os_create_task(2);//创建任务2

  // 销毁自己
  os_delete_task(0);
}

void KEY_down() {
  char dis_res = 0;
  float voltage;//ADC检测结果返回值
  float distance; // 超声波测距返回结果

  Light_on(ALL);

  Battery_get_voltage(&voltage); // 获取电源ADC检测结果

  buzzer_beep(); // 无源蜂鸣器发出声音

  dis_res = Ultrasonic_get_distance(&distance);
  if (dis_res == 0)
  {
    printf("距离获取成功: %.2f cm\n", distance);
  }else if (dis_res == 1)
  {
    printf("小于可测距离: %.2f cm\n",distance);
  }else if (dis_res == 2)
  {
    printf("大于可测距离: %.2f cm\n",distance);
  }else
  {
    printf("距离获取失败");
  }
  
  }

void KEY_up() { Light_off(ALL); }

void task1() _task_ 1 {

  while (1) {
    KEY_scan();
    os_wait2(K_TMO, 2);
  }
}

  /* void task2() _task_ 2 {

    while (1) {

    }
  } */