#include "Config.h"
#include "Delay.h"
#include "GPIO.h"
#include "NVIC.h"
#include "Timer.h"


#define BuzzerPin P00

//			 C`	   D`     E`   F`	  G`	A`	  B` C``
u16 hz[] = {1047, 1175, 1319, 1397, 1568, 1760, 1976, 2093};

// 初始化蜂鸣器GPIO
void gpio_config(void) {
  // 初始化NPN S8050的控制引脚 - P00
  P0_MODE_OUT_PP(GPIO_Pin_0); // 推挽输出
}

// 初始化定时器timer0
void Timer_config(u16 hz) {
  TIM_InitTypeDef TIM_InitStructure; // 结构定义
  // 定时器0做16位自动重装, 中断频率为1000HZ
  TIM_InitStructure.TIM_Mode =
      TIM_16BitAutoReload; // 指定工作模式,
                           // TIM_16BitAutoReload,TIM_16Bit,TIM_8BitAutoReload,TIM_16BitAutoReloadNoMask
  TIM_InitStructure.TIM_ClkSource =
      TIM_CLOCK_1T; // 指定时钟源,     TIM_CLOCK_1T,TIM_CLOCK_12T,TIM_CLOCK_Ext
  TIM_InitStructure.TIM_ClkOut = DISABLE; // 是否输出高速脉冲, ENABLE或DISABLE
  TIM_InitStructure.TIM_Value = 65536UL - (MAIN_Fosc / hz * 2); // 初值,
  TIM_InitStructure.TIM_Run = ENABLE; // 是否初始化后启动定时器, ENABLE或DISABLE
  Timer_Inilize(Timer0,
                &TIM_InitStructure); // 初始化Timer0
                                     // Timer0,Timer1,Timer2,Timer3,Timer4
  NVIC_Timer0_Init(ENABLE,
                   Priority_0); // 中断使能, ENABLE/DISABLE; 优先级(低到高)
                                // Priority_0,Priority_1,Priority_2,Priority_3
}

void time_call(void) {
  // 每一个时钟时间颠倒一次蜂鸣器的电平
  BuzzerPin = ~BuzzerPin;
}

int main() {
  u8 idx = 0;

  gpio_config();
  EA = 1;

  while (1) {
    Timer_config(hz[idx]);

    if (++idx > 7) // 先++再判断，当其值大于7时将其恢复为0
    {
      idx = 0;
    }
    delay_ms(250);
    delay_ms(250);
    delay_ms(250);
    delay_ms(250);
  }
}