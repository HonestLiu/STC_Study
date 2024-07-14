#include "Config.h"
#include "GPIO.h"
#include "Delay.h"

#include "UART.h"
#include "NVIC.h"
#include "Switch.h"
#include "PCF8563.h"
#include "oled.h"

/*初始化OLED引脚*/
void GPIO_config(void) {
    GPIO_InitTypeDef GPIO_InitStructure;        //结构定义
    GPIO_InitStructure.Pin = GPIO_Pin_2 | GPIO_Pin_3;        //指定要初始化的IO,
    GPIO_InitStructure.Mode = GPIO_OUT_OD;    //指定IO的输入或输出方式,GPIO_PullUp,GPIO_HighZ,GPIO_OUT_OD,GPIO_OUT_PP
    GPIO_Inilize(GPIO_P3, &GPIO_InitStructure);//初始化
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


int main(void) {
    char DateString[50];
    char TimeString[50];
		char TestStr[8];
		u8 cnt = 0;

    Clock_t c;
    GPIO_config();
    UART_config();
		PCF8563_init();
		
		EA = 1;

    OLED_Init();//初始化OLED
    OLED_ColorTurn(0);//0正常显示，1 反色显示
    OLED_DisplayTurn(0);//0正常显示 1 屏幕翻转显示

    // 设置时间-----------------------------------------------------------------
    // 准备数据:秒,分,时,星期,日,月,年,世纪
    c.year = 2024, c.month = 7, c.day = 2, c.week = 2;
    c.hour = 18, c.minute = 12, c.second = 01;
    // 设置时钟
    PCF8563_set(c);

    while (1) {
        // 读取时钟
        PCF8563_get(&c);
        printf("%d:%d:%d ", (int) c.hour, (int) c.minute, (int) c.second);
        //OLED显示日期和时间
        //格式化出日期 2024-7-2 09:35:22
        sprintf(DateString, "%d-%02d-%02d", (int)c.year, (int) c.month, (int) c.day);
        OLED_ShowString(0, 0, DateString, 16);
        sprintf(TimeString, "%02d:%02d:%02d", (int)c.hour, (int)c.minute, (int)c.second);
        OLED_ShowString(0,2,TimeString,16);
				cnt++;
				sprintf(TestStr,"Data:%d",(int)cnt);
				OLED_ShowString(0,4,TestStr,16);

        delay_ms(250);
        delay_ms(250);
        delay_ms(250);
        delay_ms(250);
        //清屏操作
        OLED_Clear();
    }
}