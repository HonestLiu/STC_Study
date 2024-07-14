#include "Config.h"
#include "UART.h"
#include "Delay.h"
#include "Switch.h"
#include "NVIC.h"
#include "MatrixKeys.h"


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

//按键按下的回调函数，可以在此实现具体的逻辑
void MK_on_keydown(u8 row, u8 col) {
    printf("ROW:%d COL：%d of Key Down\n", (int) row, (int) col);
}

//按键抬起的回调函数，可以在此实现具体的逻辑
void MK_on_keyup(u8 row, u8 col) {
    printf("ROW:%d COL：%d of Key Up\n", (int) row, (int) col);
}


int main() {
    MK_init();
    UART_config();

    EA = 1;
    while (1) {
        MK_scan();
        //每25ms监听一次按键的变化
        delay_ms(25);
    }
}