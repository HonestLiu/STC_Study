#include "Config.h"
#include "Delay.h"
#include "GPIO.h"

#include "ADC.h"
#include "NVIC.h"
#include "Switch.h"
#include "UART.h"

/**
使用ADC读取P05的采样值
*/

// 初始化GPIO
void gpio_config(void) {
  // ADC
  P0_MODE_IN_HIZ(GPIO_Pin_5); // 设置P05,高阻输入，用于ADC信号的采集
}

/*初始化UART*/
void uart_config(void) {
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

// 初始化ADC
void ADC_config(void) {
  ADC_InitTypeDef ADC_InitStructure; // 结构定义

  ADC_InitStructure.ADC_SMPduty = 31; // ADC 模拟信号采样时间控制, 0~31（注意：
                                      // SMPDUTY 一定不能设置小于 10）
  ADC_InitStructure.ADC_CsSetup = 0; // ADC 通道选择时间控制 0(默认),1
  ADC_InitStructure.ADC_CsHold = 1; // ADC 通道选择保持时间控制 0,1(默认),2,3
  ADC_InitStructure.ADC_Speed =
      ADC_SPEED_2X1T; // 设置 ADC 工作时钟频率	ADC_SPEED_2X1T~ADC_SPEED_2X16T
  ADC_InitStructure.ADC_AdjResult =
      ADC_RIGHT_JUSTIFIED;         // ADC结果调整,
                                   // ADC_LEFT_JUSTIFIED,ADC_RIGHT_JUSTIFIED
  ADC_Inilize(&ADC_InitStructure); // 初始化
  ADC_PowerControl(ENABLE);        // ADC电源开关, ENABLE或DISABLE
  NVIC_ADC_Init(DISABLE,
                Priority_0); // 中断使能, ENABLE/DISABLE; 优先级(低到高)
                             // Priority_0,Priority_1,Priority_2,Priority_3
}

int main() {
  u16 adc_value;    // ADC的采用值最大为12位(0~4095)
  u16 adc_value_15; // 采集ADC15通道的ADC信号
  float VRef;       // 通过ADC转换结果反推的电压值
  float VRef_15;    // 通过ADC转换结果反推的ADC15通道的电压值

  uart_config();
  gpio_config();
  ADC_config();

  EA = 1; // 使能全局中断

  while (1) {
    // 读取ADC转换结果
    adc_value = Get_ADCResult(
        ADC_CH13); // 通道是根据实际使用的端口而定的，这里P05对应的是CH13
    adc_value_15 = Get_ADCResult(ADC_CH15);

    // 通过公式反推出ADC输入电压(测量电压)
    VRef = 2.5 * adc_value / 4096;
    VRef_15 = 2.5 * adc_value / 4096;

    // 打印测量结果
    printf("ADC:%d VRef:%f\n", adc_value, VRef);
    printf("ADC_15:%d VRef_15:%f\n", adc_value_15, VRef_15);

    delay_ms(250);
    delay_ms(250);
    delay_ms(250);
    delay_ms(250);
  }
}
