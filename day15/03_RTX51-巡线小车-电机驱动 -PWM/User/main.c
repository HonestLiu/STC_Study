// 外部设备
#include "../Drive/Buzzer.h"
#include "../Drive/Key.h"
#include "../Drive/Light.h"
#include "../Drive/Track.h"
#include "../Drive/Ultrasonic.h"
#include "../Drive/battery.h"
// 片上外设
#include "../Lib/Config.h"
#include "../Lib/GPIO.h"
#include "../Lib/NVIC.h"
#include "../Lib/STC8H_PWM.h"
#include "../Lib/Switch.h"
#include "../Lib/UART.h"

#include "RTX51TNY.h" //RTX51的系统头文件

#define RQ_B P15
#define RQ_F P14

#define PERIOD (MAIN_Fosc / 1000)
PWMx_Duty dutyA;
void PWM_config(void) {
  PWMx_InitDefine PWMx_InitStructure;
  // 配置PWM3
  PWMx_InitStructure.PWM_Mode =
      CCMRn_PWM_MODE1; // 模式,
                       // CCMRn_FREEZE,CCMRn_MATCH_VALID,CCMRn_MATCH_INVALID,CCMRn_ROLLOVER,CCMRn_FORCE_INVALID,CCMRn_FORCE_VALID,CCMRn_PWM_MODE1,CCMRn_PWM_MODE2
  PWMx_InitStructure.PWM_Duty =
      PERIOD * 50 / 100; // PWM占空比时间->占整个周期的50%
  PWMx_InitStructure.PWM_EnoSelect =
      ENO3P |
      ENO3N; // 输出通道选择,	ENO1P,ENO1N,ENO2P,ENO2N,ENO3P,ENO3N,ENO4P,ENO4N
             // / ENO5P,ENO6P,ENO7P,ENO8P
  PWM_Configuration(PWM3, &PWMx_InitStructure);

  // 配置PWMA
  PWMx_InitStructure.PWM_Period = PERIOD - 1;    // 周期时间,   0~65535
  PWMx_InitStructure.PWM_DeadTime = 0;           // 死区发生器设置, 0~255
  PWMx_InitStructure.PWM_MainOutEnable = ENABLE; // 主输出使能, ENABLE,DISABLE
  PWMx_InitStructure.PWM_CEN_Enable = ENABLE; // 使能计数器, ENABLE,DISABLE
  PWM_Configuration(PWMA,
                    &PWMx_InitStructure); // 初始化PWM通用寄存器,  PWMA,PWMB

  // 切换PWM通道
  PWM3_SW(PWM3_SW_P14_P15); // PWM3_SW_P14_P15,PWM3_SW_P24_P25,PWM3_SW_P64_P65

  // 初始化PWMA的中断
  NVIC_PWM_Init(PWMA, DISABLE, Priority_0);
}

void GPIO_Config(void) {
  // 初始化为准双向即可
  P1_MODE_IO_PU(GPIO_Pin_4 | GPIO_Pin_5);
}

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
  GPIO_Config();
  PWM_config();

  // 驱动初始化
  Light_init();      // 初始化转向灯
  KEY_init();        // 初始化按钮
  Battery_init();    // 初始化ADC电源检测
  buzzer_init();     // 初始化蜂鸣器
  Ultrasonic_init(); // 初始化超声波模块
  Track_init();      // 初始化巡线模块

  EA = 1;
}

void start_main() _task_ 0 {
  sys_init();
  os_create_task(1); // 创建按键任务
  // os_create_task(2); // 创建巡线任务

  // 销毁自己
  os_delete_task(0);
}

u8 d3 = 50; // 电机的占空比 0 ~ 100
int direction_step = 10;
void KEY_down() {
  Light_on(ALL);
  // 按键按下，调整占空比
  d3 += direction_step;
  if (d3 == 100) {
    direction_step = -10; // 开始递减
  } else if (d3 <= 0) {
    direction_step = 10; // 开始递增
  }
  dutyA.PWM3_Duty = PERIOD * d3 / 100;
  UpdatePwm(PWM3, &dutyA);
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