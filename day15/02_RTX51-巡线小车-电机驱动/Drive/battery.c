#include "battery.h"
#include "../Lib/ADC.h"
#include "../Lib/GPIO.h"
#include "../Lib/NVIC.h"

#define ADC_GPIO_PIN GPIO_Pin_1
#define ADC_GPIO GPIO_P0

/******************* GPIO初始化函数 *******************/
static void GPIO_config(void) {
  GPIO_InitTypeDef gpio_init_struct;
  gpio_init_struct.Mode = GPIO_HighZ;
  gpio_init_struct.Pin = ADC_GPIO_PIN;
  GPIO_Inilize(ADC_GPIO, &gpio_init_struct);
}

/******************* AD配置函数 *******************/
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

void Battery_init() {
  GPIO_config();
  ADC_config();
}

void Battery_get_voltage(float *voltage) {
  float value;
  // 获取ADC转换结果
  u16 result = Get_ADCResult(
      ADC_CH9); // 通道是根据实际使用的端口而定的，这里P05对应的是CH13
  // 根据公式计算被转换通道输入电压值
  value = 2.5 * result / 4096;
  // 实际应用电路串联了3个相同电阻，串联分压，乘个3就可以得到要测量端实际输入电压了
  value *= 3.0f;
  //加上电池流经二极管的分压(需要实际测量其具体分压值再加上)
  value += 0.8;
  *voltage = value;
}
