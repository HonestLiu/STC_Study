#include "../Lib/Config.h"
#include "../Lib/Delay.h"
#include "../Lib/Exti.h"
#include "../Lib/GPIO.h"
#include "../Lib/NVIC.h"
#include "../Lib/STC8H_PWM.h"
#include "../Lib/Switch.h"
#include "../Lib/UART.h"

/**
 * 初始化GPIO
 * - PWM6的P21
 * - Key的P32(板载),外部中断0引脚
 */
void GPIO_Config() {
  GPIO_InitTypeDef gpio_init_struct;

  // 初始化PWM引脚
  gpio_init_struct.Mode = GPIO_PullUp; // 准双向
  gpio_init_struct.Pin = GPIO_Pin_1;
  GPIO_Inilize(GPIO_P2, &gpio_init_struct);
  // 初始化KEY引脚
  gpio_init_struct.Mode = GPIO_PullUp; // 准双向
  gpio_init_struct.Pin = GPIO_Pin_2;
  GPIO_Inilize(GPIO_P3, &gpio_init_struct);
}

// 初始化UART
void UART_config(void) {
  COMx_InitDefine COMx_InitStructure;
  COMx_InitStructure.UART_Mode = UART_8bit_BRTx;
  COMx_InitStructure.UART_BRT_Use = BRT_Timer1;
  COMx_InitStructure.UART_BaudRate = 115200ul;
  COMx_InitStructure.UART_RxEnable = ENABLE;
  COMx_InitStructure.BaudRateDouble = DISABLE;
  UART_Configuration(UART1, &COMx_InitStructure);

  NVIC_UART1_Init(ENABLE, Priority_1);
  UART1_SW(UART1_SW_P30_P31);
}

#define FREQ 50                                 // 要输出的PWM频率
#define Prescaler 10                            // 分频系数
#define PERIOD (MAIN_Fosc / (FREQ * Prescaler)) // 重装载寄存器的值
void PWM_config(void) {
  PWMx_InitDefine PWMx_InitStructure;

  // 配置PWM6
  PWMx_InitStructure.PWM_Mode = CCMRn_PWM_MODE1;
  PWMx_InitStructure.PWM_Duty = 0;
  PWMx_InitStructure.PWM_EnoSelect = ENO6P;
  PWM_Configuration(PWM6, &PWMx_InitStructure);

  // 配置PWMB
  PWMx_InitStructure.PWM_Period = PERIOD - 1;    // 周期时间
  PWMx_InitStructure.PWM_DeadTime = 0;           // 死区发生器设置, 0~255
  PWMx_InitStructure.PWM_MainOutEnable = ENABLE; // 主输出使能, ENABLE,DISABLE
  PWMx_InitStructure.PWM_CEN_Enable = ENABLE; // 使能计数器, ENABLE,DISABLE
  PWM_Configuration(PWMB, &PWMx_InitStructure); // 初始化PWM通用寄存器

  // 给PWMB设置预分频系数
  PWMB_Prescaler(Prescaler - 1);

  // 切换PWM通道
  PWM6_SW(PWM6_SW_P21); // PWM6_SW_P21,PWM6_SW_P54,PWM6_SW_P01,PWM6_SW_P75

  // 初始化PWMB的中断
  NVIC_PWM_Init(PWMB, DISABLE, Priority_0);
}

// 初始化外部中断
void Exti_config(void) {
  EXTI_InitTypeDef Exti_InitStructure;

  Exti_InitStructure.EXTI_Mode = EXT_MODE_Fall;
  Ext_Inilize(EXT_INT0, &Exti_InitStructure);
  NVIC_INT0_Init(ENABLE, Priority_0);
}

PWMx_Duty dutyB;
u16 duty_value = 500;
void ext_int0_call(void) {
  static int step = 500;
  float angle = 0;
  // 当中断触发时的实现逻辑
  delay_ms(5); // 防抖
  if (P32 != 0) {
    return;
  }
  duty_value += step;

  if (duty_value >= 2500) {
    step = -500;
  } else if (duty_value <= 500) {
    step = +500;
  }
  printf("Key_Down,duty=%d\n", duty_value);
  // duty范围: 500us ~ 2500us
  // -500 >> 0us ~ 500us
  // ÷2000 >> 0 ~ 1.0
  // * 180 >>即可得到对应的角度了
  angle = ((duty_value - 500) * 180.0f) / 2000;
  printf("angle: %.2f\n", angle);

  // 在这里更新占空比即可控制舵机
  // Duty = 48,000(20ms) * (500 /20000) = 20ms * 0.025 = 0.5ms
  dutyB.PWM6_Duty = PERIOD * duty_value / 20000;
  UpdatePwm(PWM6, &dutyB);
}

int main() {
  EAXSFR(); /* 扩展寄存器访问使能 */

  GPIO_Config();
  UART_config();
  Exti_config();
  PWM_config();

  EA = 1;

  dutyB.PWM6_Duty = PERIOD * 500 / 20000;
  UpdatePwm(PWM6, &dutyB);

  while (1) {
  }
}