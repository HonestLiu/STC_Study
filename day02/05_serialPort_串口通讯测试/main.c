#include "Config.h"
#include "GPIO.h"
#include "UART.h"
#include "NVIC.h"
#include "Delay.h"
#include "Switch.h"

/**
 * @author HonestLiu
 * @version 1.0
 * @Description: 串口通讯测试，通过UART1接收数据，然后通过UART4发送出去
 * @note UART1引脚: RXD(P3.0)和TXD(P3.1) UART2引脚:RXD(P5.2)和TXD(P5.3),这是一个通用的代码，可以同时烧写到两块实验板上，其会判断不同端口进而执行不同逻辑
 * @since 2024/6/13
 */

// 配置GPIO
void gpio_config(void)
{

    /* UART1引脚初始化 */
    GPIO_InitTypeDef gpioConfigStruct;
    gpioConfigStruct.Pin = GPIO_Pin_0 | GPIO_Pin_1; // 配置引脚0和1
    gpioConfigStruct.Mode = GPIO_OUT_PP;            // 设置为推挽输出模式
    GPIO_Inilize(GPIO_P3, &gpioConfigStruct);

    /* UART2引脚初始化 */
    gpioConfigStruct.Pin = GPIO_Pin_2 | GPIO_Pin_3; // 配置引脚0和1
    gpioConfigStruct.Mode = GPIO_OUT_PP;            // 设置为推挽输出模式
    GPIO_Inilize(GPIO_P5, &gpioConfigStruct);
}

// 配置NVIC
void nvic_config(void)
{
    COMx_InitDefine COMx_InitStructure; // 结构定义

    COMx_InitStructure.UART_Mode = UART_8bit_BRTx;  // 模式, UART_ShiftRight,UART_8bit_BRTx,UART_9bit,UART_9bit_BRTx
    COMx_InitStructure.UART_BRT_Use = BRT_Timer1;   // 选择波特率发生器, BRT_Timer1, BRT_Timer2 (注意: 串口2固定使用BRT_Timer2)
    COMx_InitStructure.UART_BaudRate = 115200ul;    // 波特率, 一般 110 ~ 115200
    COMx_InitStructure.UART_RxEnable = ENABLE;      // 接收允许,   ENABLE或DISABLE
    COMx_InitStructure.BaudRateDouble = DISABLE;    // 波特率加倍, ENABLE或DISABLE
    UART_Configuration(UART1, &COMx_InitStructure); // 初始化串口1 UART1,UART2,UART3,UART4
    UART_Configuration(UART4, &COMx_InitStructure); // 初始化串口4(如果没高亮记得去打开相应的宏)
    NVIC_UART1_Init(ENABLE, Priority_1);            // 中断使能UART1, ENABLE/DISABLE; 优先级(低到高) Priority_0,Priority_1,Priority_2,Priority_3
    NVIC_UART4_Init(ENABLE, Priority_1);            // 中断使能UART4

    // 设置UART1和UART4的引脚
    UART1_SW(UART1_SW_P30_P31); // RXD(P3.0)和TXD(P3.1)
    UART4_SW(UART4_SW_P52_P53); // RXD(P5.2)和TXD(P5.3)
}

// 数据读取发送(回塞)函数-UART1
// 接收到数据发送给另一块板子
void backPlug01(void)
{
    u8 i;
    u8 dat;
    // 按照读取字节的字节数变量缓冲区获取数据，并将获取的数据发送回去
    for (i = 0; i < COM1.RX_Cnt; i++)
    {
        dat = RX1_Buffer[i];
        TX4_write2buff(dat); // 将收到的数据通过串口4发送出去
    }
}

// 数据读取发送(回塞)函数-UART4
// 接收到数据发送给PC
void backPlug04(void)
{
    u8 i;
    u8 dat;
    // 按照读取字节的字节数变量缓冲区获取数据，并将获取的数据发送回去
    for (i = 0; i < COM4.RX_Cnt; i++)
    {
        dat = RX4_Buffer[i];
        TX1_write2buff(dat); // 将收到的数据通过串口1发送出去,发送给PC
    }
}

int main(void)
{
    // 初始化GPIO
    gpio_config();
    // 初始化NVIC
    nvic_config();
    // 开启中断
    EA = 1;

    while (1)
    {
        /* UART1收到数据的处理逻辑 */
        if ((COM1.RX_TimeOut > 0) && (--COM1.RX_TimeOut == 0))
        {
            // 接收到的字节数>0,证明确实是收到数据了
            if (COM1.RX_Cnt > 0)
            {
                // 调用数据读取函数读取获取到的数据
                backPlug01();
            }
            // 读取完数据，将记录接收数据字节的变量置0
            COM1.RX_Cnt = 0;
        }

        /* UART4收到数据的处理逻辑 */
        if ((COM4.RX_TimeOut > 0) && (--COM4.RX_TimeOut == 0))
        {
            if (COM4.RX_Cnt > 0)
            {
                backPlug04();
            }
            COM4.RX_Cnt = 0;
        }

        // 延时10ms,避免频繁读取数据
        delay_ms(10);
    }
}
