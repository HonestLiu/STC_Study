#include "Config.h"
#include "Delay.h"
#include "GPIO.h"
#include "Exti.h"
#include "NVIC.h"
#include "UART.h"
#include "Switch.h"

/****************  GPIO初始化函数 *****************/
void GPIO_config(void) {
    //初始化中断监听管脚 -P32
    GPIO_InitTypeDef gpio_init_struct;
    gpio_init_struct.Mode = GPIO_PullUp;
    gpio_init_struct.Pin = GPIO_Pin_2;
    GPIO_Inilize(GPIO_P3, &gpio_init_struct);
}

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

/******************** INT配置 ********************/
void Exti_config(void) {
    EXTI_InitTypeDef Exti_InitStructure;                            //结构定义

    Exti_InitStructure.EXTI_Mode = EXT_MODE_RiseFall;//中断模式,   EXT_MODE_RiseFall,EXT_MODE_Fall
    Ext_Inilize(EXT_INT0, &Exti_InitStructure);                //初始化中断1，对应引脚是p32
    NVIC_INT0_Init(ENABLE,
                   Priority_0);        //中断使能, ENABLE/DISABLE; 优先级(低到高) Priority_0,Priority_1,Priority_2,Priority_3
}

void ext_int0_call(void) {
    // 监听的沿设置的虽然是上升下降沿，但我们可以通过判断P32的最新电平状态判断对其进行区分
    if (P32) {
        printf("int0_call:Rising edge\n");//上升沿
    } else {
        printf("int0_call:Falling edge\n");//下降沿
    }
}

int main() {

    GPIO_config();
    UART_config();
    Exti_config();

    EA = 1;//使能中断

    while (1) {


        delay_ms(250);
        delay_ms(250);
        delay_ms(250);
        delay_ms(250);
    }


}
