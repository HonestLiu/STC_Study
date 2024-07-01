#ifndef __NTC_H_
#define __NTC_H_

#include "Config.h"

/* 预留了宏定义，移植的时候直接修改这里成需要的参数即可 */

// ADC GPIO配置
#define NTC_GPIO GPIO_P0
#define NTC_GPIO_PIN GPIO_Pin_4
// ADC 通道配置
#define NTC_ACD_CH ADC_CH12
// 基准电压配置
#define VOL_REF 2.5

void ntc_init(void);

int NTC_get_temperature(void);

#endif