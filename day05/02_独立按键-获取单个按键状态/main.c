#include "Config.h"
#include "GPIO.h"
#include "UART.h"
#include "NVIC.h"
#include "Switch.h"
#include <stdio.h>

#define KEY1 P51
#define LED P04

void gpio_config(void) {
  // 初始化独立按键的相关引脚
  P5_MODE_IO_PU(GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5);//准双向
  // 初始化测试用LED引脚
  P0_MODE_OUT_PP(GPIO_Pin_4);//推挽输出
}

void UART_config(void) {
	// >>> 记得添加 NVIC.c, UART.c, UART_Isr.c <<<
    COMx_InitDefine		COMx_InitStructure;					//结构定义
    COMx_InitStructure.UART_Mode      = UART_8bit_BRTx;	//模式, UART_ShiftRight,UART_8bit_BRTx,UART_9bit,UART_9bit_BRTx
    COMx_InitStructure.UART_BRT_Use   = BRT_Timer1;			//选择波特率发生器, BRT_Timer1, BRT_Timer2 (注意: 串口2固定使用BRT_Timer2)
    COMx_InitStructure.UART_BaudRate  = 115200ul;			//波特率, 一般 110 ~ 115200
    COMx_InitStructure.UART_RxEnable  = ENABLE;				//接收允许,   ENABLE或DISABLE
    COMx_InitStructure.BaudRateDouble = DISABLE;			//波特率加倍, ENABLE或DISABLE
    UART_Configuration(UART1, &COMx_InitStructure);		//初始化串口1 UART1,UART2,UART3,UART4

  	NVIC_UART1_Init(ENABLE,Priority_1);		//中断使能, ENABLE/DISABLE; 优先级(低到高) Priority_0,Priority_1,Priority_2,Priority_3
    UART1_SW(UART1_SW_P30_P31);		// 引脚选择, UART1_SW_P30_P31,UART1_SW_P36_P37,UART1_SW_P16_P17,UART1_SW_P43_P44
}

u8 key_old_state = 0;//记录按键的状态

int main() {
    gpio_config();
    UART_config();
    EA = 1;
	

    while (1) {
        //按键按下
        if (KEY1 == 1 && key_old_state == 1 )
        {   
            printf("key uplift\r\n");
            key_old_state = 0;
        //按键抬起
        }   else if (KEY1 == 0 && key_old_state == 0) {
            printf("key down\r\n");
            key_old_state = 1;
						LED = !LED;//取反LED，改变它的状态
        }
    }

}