#include "Config.h"
#include "Delay.h"
#include "NVIC.h"
#include "Switch.h"
#include "UART.h"
#include "DHT11.h"

/**
 * @author HonestLiu
 * @webSite honestliu.com
 * @mail mail\@honestliu.com
 * @description 获取DHT11的数据并解析-封装版，默认DHT11数据管脚为P46,可前往DHT11.h进行修改
 * */

// 初始化UART
void UART_config(void) {
    // >>> 记得添加 NVIC.c, UART.c, UART_Isr.c <<<
    COMx_InitDefine COMx_InitStructure;                    //结构定义
    COMx_InitStructure.UART_Mode = UART_8bit_BRTx;    //模式, UART_ShiftRight,UART_8bit_BRTx,UART_9bit,UART_9bit_BRTx
    COMx_InitStructure.UART_BRT_Use = BRT_Timer1;            //选择波特率发生器, BRT_Timer1, BRT_Timer2 (注意: 串口2固定使用BRT_Timer2)
    COMx_InitStructure.UART_BaudRate = 115200ul;            //波特率, 一般 110 ~ 115200
    COMx_InitStructure.UART_RxEnable = ENABLE;                //接收允许,   ENABLE或DISABLE
    COMx_InitStructure.BaudRateDouble = DISABLE;            //波特率加倍, ENABLE或DISABLE
    UART_Configuration(UART1, &COMx_InitStructure);        //初始化串口1 UART1,UART2,UART3,UART4

    NVIC_UART1_Init(ENABLE,
                    Priority_1);        //中断使能, ENABLE/DISABLE; 优先级(低到高) Priority_0,Priority_1,Priority_2,Priority_3
    UART1_SW(UART1_SW_P30_P31);        // 引脚选择, UART1_SW_P30_P31,UART1_SW_P36_P37,UART1_SW_P16_P17,UART1_SW_P43_P44
}


int main() {
    float humidity = 0;
    float temperature = 0;

    DHT11_init();//初始化DHT11
    UART_config();

    EA = 1;//开启中断

    while (1) {
        if (COM1.RX_TimeOut > 0) {
            //超时计数
            if (--COM1.RX_TimeOut == 0) {
                if (COM1.RX_Cnt > 0) {
                    if ((DHT11_get_info(&humidity, &temperature)) == 0) {//获取DHT11的数据并判断是否获取成功
                        printf("Humidity:%.2f %% \n", humidity);
                        printf("Temperature:%.2f C\n", temperature);
                    } else {
                        printf("Data validation failure!!");//数据校验失败
                    }
                }
                COM1.RX_Cnt = 0;
            }
        }
        // 不要处理的太快
        delay_ms(10);
    }
}