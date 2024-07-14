#include "Config.h"
#include "GPIO.h"
#include "UART.h"
#include "Delay.h"
#include "Switch.h"
#include "NVIC.h"

#define Row1 P34
#define Row2 P35
#define Row3 P40
#define Row4 P41
#define COL1 P03
#define COL2 P06
#define COL3 P07
#define COL4 P17


//将矩阵键盘的相关引脚全部初始化为准双向
void GPIO_config() {
    P0_MODE_IO_PU(GPIO_Pin_3 | GPIO_Pin_6 | GPIO_Pin_7);
    P1_MODE_IO_PU(GPIO_Pin_7);
    P3_MODE_IO_PU(GPIO_Pin_4 | GPIO_Pin_5);
    P4_MODE_IO_PU(GPIO_Pin_0 | GPIO_Pin_1);
}

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

//按下为低电平，抬起为高电平
#define DOWN 0
#define UP 1

u8 last_state = UP;

void key_scan() {
    //初始化矩阵键盘行的电平，这里只监听的第一行，所以只将其输出化为低电平
    Row1 = 0;
    Row2 = 1;
    Row3 = 1;
    Row4 = 1;
    //按键按下
    if (COL1 == DOWN && last_state == UP) {
        printf("Key Down\n");
        //更新状态
        last_state = DOWN;
    } else if (COL1 == UP && last_state == DOWN) {
        printf("Key Up\n");
        last_state = UP;
    }
}

int main() {
    GPIO_config();
    UART_config();

    EA = 1;
    while (1) {
        key_scan();
        //每25ms监听一次按键的变化
        delay_ms(25);
    }
}