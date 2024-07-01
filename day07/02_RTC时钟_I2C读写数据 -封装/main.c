#include "Config.h"
#include "Delay.h"
#include "NVIC.h"
#include "UART.h"
#include "Switch.h"
#include "PCF8563.h"

/****************  UART初始化函数 *****************/
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
    //存储转换后的值
    Clock_t c;

    PCF8563_init();//初始化I2C
    UART_config();

    EA = 1;//使能中断

    /*1.设置RTC芯片时间*/
    //准备要设置的数据
    c.year = 2024, c.month = 6, c.day = 29, c.week = 1, c.hour = 23, c.minute = 59, c.second = 54;
    //进行时间设置
    PCF8563_set(&c);

    while (1) {
        /*2.循环读取RTC芯片的数据*/
        PCF8563_get(&c);

        //printf("Hour:%d;Minute:%d;c.second: %d\n", (int) hour, (int) minute, (int) c.second);
        printf("%d-%d-%d-%d-%d-%d,Week:%d\n", (int) c.year, (int) c.month, (int) c.day, (int) c.hour, (int) c.minute,
               (int) c.second, (int) c.week);

        delay_ms(250);
        delay_ms(250);
        delay_ms(250);
        delay_ms(250);
    }


}
