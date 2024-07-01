#include "ADC.h"
#include "Config.h"
#include "Delay.h"
#include "GPIO.h"
#include "NVIC.h"
#include "Switch.h"
#include "UART.h"
#include <stdio.h>
/**
 * @note 需求: 测量P04端口连接的热敏电阻的阻值
 */

#define R20 10

/* 初始化GPIO P04 为浮空输入模式，即ADC引脚 */
void Gpio_Config(void) {
  GPIO_InitTypeDef gpio_config_struct;
  gpio_config_struct.Mode = GPIO_HighZ; // 浮空输入
  gpio_config_struct.Pin = GPIO_Pin_4;  // 引脚4
  GPIO_Inilize(GPIO_P0, &gpio_config_struct);
}

/* 初始化Uart */
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

/* 初始化ADC */
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

/**
 * 说明: 存储在MCU代码区的温度对照表，使用u16存储是为了节省内存(小数更占内存)
 * 使用:
 * 将要查询的温度放大100倍传入这个数组进行大小比较，找到与其最接近数的索引，用该索引减去55就能得到温度了
 */
u16 code temp_table[] = {
    58354, // -55
    55464, // -54
    52698, // -53
    50048, // -52
    47515, // -51
    45097, // -50
    42789, // -49
    40589, // -48
    38492, // -47
    36496, // -46
    34597, // -45
    32791, // -44
    31075, // -43
    29444, // -42
    27896, // -41
    26427, // -40
    25034, // -39
    23713, // -38
    22460, // -37
    21273, // -36
    20148, // -35
    19083, // -34
    18075, // -33
    17120, // -32
    16216, // -31
    15361, // -30
    14551, // -29
    13785, // -28
    13061, // -27
    12376, // -26
    11728, // -25
    11114, // -24
    10535, // -23
    9986,  // -22
    9468,  // -21
    8977,  // -20
    8513,  // -19
    8075,  // -18
    7660,  // -17
    7267,  // -16
    6896,  // -15
    6545,  // -14
    6212,  // -13
    5898,  // -12
    5601,  // -11
    5319,  // -10
    5053,  // -9
    4801,  // -8
    4562,  // -7
    4336,  // -6
    4122,  // -5
    3920,  // -4
    3728,  // -3
    3546,  // -2
    3374,  // -1
    3211,  // 0
    3057,  // 1
    2910,  // 2
    2771,  // 3
    2639,  // 4
    2515,  // 5
    2396,  // 6
    2284,  // 7
    2177,  // 8
    2076,  // 9
    1978,  // 10
    1889,  // 11
    1802,  // 12
    1720,  // 13
    1642,  // 14
    1568,  // 15
    1497,  // 16
    1430,  // 17
    1366,  // 18
    1306,  // 19
    1248,  // 20
    1193,  // 21
    1141,  // 22
    1092,  // 23
    1044,  // 24
    1000,  // 25
    957,   // 26
    916,   // 27
    877,   // 28
    840,   // 29
    805,   // 30
    771,   // 31
    739,   // 32
    709,   // 33
    679,   // 34
    652,   // 35
    625,   // 36
    600,   // 37
    576,   // 38
    552,   // 39
    530,   // 40
    509,   // 41
    489,   // 42
    470,   // 43
    452,   // 44
    434,   // 45
    417,   // 46
    401,   // 47
    386,   // 48
    371,   // 49
    358,   // 50
    344,   // 51
    331,   // 52
    318,   // 53
    306,   // 54
    295,   // 55
    284,   // 56
    274,   // 57
    264,   // 58
    254,   // 59
    245,   // 60
    236,   // 61
    228,   // 62
    220,   // 63
    212,   // 64
    205,   // 65
    198,   // 66
    191,   // 67
    184,   // 68
    178,   // 69
    172,   // 70
    166,   // 71
    160,   // 72
    155,   // 73
    150,   // 74
    145,   // 75
    140,   // 76
    135,   // 77
    131,   // 78
    126,   // 79
    122,   // 80
    118,   // 81
    115,   // 82
    111,   // 83
    107,   // 84
    104,   // 85
    101,   // 86
    97,    // 87
    94,    // 88
    91,    // 89
    89,    // 90
    86,    // 91
    83,    // 92
    81,    // 93
    78,    // 94
    76,    // 95
    74,    // 96
    71,    // 97
    69,    // 98
    67,    // 99
    65,    // 100
    63,    // 101
    61,    // 102
    60,    // 103
    58,    // 104
    56,    // 105
    55,    // 106
    53,    // 107
    52,    // 108
    50,    // 109
    49,    // 110
    47,    // 111
    46,    // 112
    45,    // 113
    43,    // 114
    42,    // 115
    41,    // 116
    40,    // 117
    39,    // 118
    38,    // 119
    37,    // 120
    36,    // 121
    35,    // 122
    34,    // 123
    33,    // 124
    32,    // 125
};

#define ABS(x) x > 0 ? (x) : -(x)

/**
 * @parm search_ntc 要查询对应温度的阻值(需乘100传入)
 * @return 要查询的温度
 */
int NTC_get_temperature(float search_ntc) {
  int i;
  int min_index = 0; // 最小索引
  float diff;        // 温度值比较的临时索引

  // 计算温度对照表数组的长度
  int temp_size = sizeof(temp_table) / sizeof(temp_table[min_index]);
  // 记录最小温度值
  float min_diff = ABS(search_ntc - temp_table[0]);
  // 遍历温度数组，计算与其接近值的最小索引
  for (i = 0; i < temp_size; i++) {
    diff = ABS(search_ntc - temp_table[i]);
    // 如果当前遍历值小于定义的最小温度值，那么就让他成为最小值
    if (diff < min_diff) {
      min_diff = diff;
      min_index = i;
    }
  }

  return (min_index - 55);
}

int main(void) {
  u16 adc_value; // ADC信号
  float v_ntc;   // NTC的电压
  float R_ntc;   // NTC的阻值
  int temp;      // NTC阻值所对应的温度

  Gpio_Config();
  UART_config();
  ADC_config();

  EA = 1; // 开启全局中断

  while (1) {
    adc_value = Get_ADCResult(ADC_CH12); // 从ADC12通道获取ADC值，对应P04
    // 计算热敏电阻的电压
    v_ntc = adc_value * 2.5 / 4096;
    // 根据电路设计及欧姆定理计算热敏电阻的阻值
    R_ntc = (v_ntc * R20) / (5 - v_ntc);
    // 查询阻值对应的温度值
    temp = NTC_get_temperature(R_ntc * 100);
    // 打印阻值
    printf("R_ntc: %.2fR Temp: %dC", R_ntc, temp);

    delay_ms(250);
    delay_ms(250);
    delay_ms(250);
    delay_ms(250);
  }
}