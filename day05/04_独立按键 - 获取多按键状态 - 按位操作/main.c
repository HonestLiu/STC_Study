#include "Config.h"
#include "GPIO.h"
#include "UART.h"
#include "NVIC.h"
#include "Switch.h"
#include <stdio.h>

#define KEY0 P51
#define KEY1 P52
#define KEY2 P53
#define KEY3 P54

#define LED P04
/* 按键状态宏 */
#define DOWN 0
#define UP 1

u8 key_state = 0x0F;//记录KEY的最后一个状态的8个位

/* 判断按键状态的宏函数 */
#define KEY_IS_DOWN(x) (key_state & (1 << x)) == 0
#define KEY_IS_UP(x) (key_state & (1 << x)) > 0
/* 设置按键状态的宏函数 */
#define KEY_SET_DOWN(x) (key_state &= ~(1 << x))
#define KEY_SET_UP(x) (key_state |= (1 << x))


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


int main() {
    gpio_config();
    UART_config();
    EA = 1;
	

    while (1) {
        /* 按键0 */

        //按键抬起,按键电平状态为 高 且上次标记状态为 按下
        if (KEY0 && KEY_IS_DOWN(0) )
        {   
            printf("key0 uplift\r\n");
            KEY_SET_UP(0);
            LED = !LED;//取反LED，改变它的状态
        //按键按下，按键电平状态为 低 且上次标记状态为 抬起
        }   else if (!KEY0 && KEY_IS_UP(0)) {
            printf("key0 down\r\n");
            KEY_SET_DOWN(0);
        }

        /* 按键1 */
        //按键抬起,按键电平状态为 高 且上次标记状态为 按下
        if (KEY1 && KEY_IS_DOWN(1) )
        {   
            printf("key1 uplift\r\n");
            KEY_SET_UP(1);
            LED = !LED;//取反LED，改变它的状态
        //按键按下，按键电平状态为 低 且上次标记状态为 抬起
        }   else if (!KEY1 && KEY_IS_UP(1)) {
            printf("key1 down\r\n");
            KEY_SET_DOWN(1);
        }

        /* 按键2 */
        //按键抬起,按键电平状态为 高 且上次标记状态为 按下
        if (KEY2 && KEY_IS_DOWN(2) )
        {   
            printf("key2 uplift\r\n");
            KEY_SET_UP(2);
            LED = !LED;//取反LED，改变它的状态
        //按键按下，按键电平状态为 低 且上次标记状态为 抬起
        }   else if (!KEY2 && KEY_IS_UP(2)) {
            printf("key2 down\r\n");
            KEY_SET_DOWN(2);
        }

        /* 按键3 */
         //按键抬起,按键电平状态为 高 且上次标记状态为 按下
        if (KEY3 && KEY_IS_DOWN(3) )
        {   
            printf("key3 uplift\r\n");
            KEY_SET_UP(3);
            LED = !LED;//取反LED，改变它的状态
        //按键按下，按键电平状态为 低 且上次标记状态为 抬起
        }   else if (!KEY3&& KEY_IS_UP(3)) {
            printf("key3 down\r\n");
            KEY_SET_DOWN(3);
        }
    }

}