#include "Config.h"
#include "GPIO.h"
#include "UART.h"
#include "NVIC.h"
#include "Delay.h"
#include "Switch.h"

// 配置GPIO
void gpio_config(void)
{
    GPIO_InitTypeDef gpioConfigStruct;
    gpioConfigStruct.Pin = GPIO_Pin_0 | GPIO_Pin_1; // 配置引脚0和1
    gpioConfigStruct.Mode = GPIO_OUT_PP;			// 设置为推挽输出模式
    GPIO_Inilize(GPIO_P3, &gpioConfigStruct);
}

// 配置NVIC
void nvic_config(void)
{
    COMx_InitDefine COMx_InitStructure; // 结构定义

    COMx_InitStructure.UART_Mode = UART_8bit_BRTx;	// 模式, UART_ShiftRight,UART_8bit_BRTx,UART_9bit,UART_9bit_BRTx
    COMx_InitStructure.UART_BRT_Use = BRT_Timer1;	// 选择波特率发生器, BRT_Timer1, BRT_Timer2 (注意: 串口2固定使用BRT_Timer2)
    COMx_InitStructure.UART_BaudRate = 115200ul;	// 波特率, 一般 110 ~ 115200
    COMx_InitStructure.UART_RxEnable = ENABLE;		// 接收允许,   ENABLE或DISABLE
    COMx_InitStructure.BaudRateDouble = DISABLE;	// 波特率加倍, ENABLE或DISABLE
    UART_Configuration(UART1, &COMx_InitStructure); // 初始化串口1 UART1,UART2,UART3,UART4
    NVIC_UART1_Init(ENABLE, Priority_1);			// 中断使能, ENABLE/DISABLE; 优先级(低到高) Priority_0,Priority_1,Priority_2,Priority_3

    UART1_SW(UART1_SW_P30_P31);
}

// 数据读取发送(回塞)函数
void backPlug(void)
{
    u8 i;
    u8 dat;
    // 按照读取字节的字节数变量缓冲区获取数据，并将获取的数据发送回去
    for (i = 0; i < COM1.RX_Cnt; i++)
    {
        dat = RX1_Buffer[i];
        TX1_write2buff(dat); // 将收到的数据使用TX1发回去(给PC)
    }
}

int main(void)
{
    //初始化GPIO
    gpio_config();
    //初始化NVIC
    nvic_config();
    // 开启中断
    EA = 1;

    while (1)
    {
        /* 超时计数，一旦接收到一个字节，RX_TimeOut就会初始化一个值作为超时值，超过此事件视作超时 */
        // 超时值有值，证明可能收到数据了，进行进一步判断
        if ((COM1.RX_TimeOut > 0) && (--COM1.RX_TimeOut == 0))
        {
            // 接收到的字节数>0,证明确实是收到数据了
            if (COM1.RX_Cnt > 0)
            {
                // 调用数据读取函数读取获取到的数据
                backPlug();
            }
            // 读取完数据，将记录接收数据字节的变量置0
            COM1.RX_Cnt = 0;
        }
        // 延时10ms,避免频繁读取数据
        delay_ms(10);
    }
}
