#include "Config.h"
#include "GPIO.h"
#include "Delay.h"
#include "Timer.h"
#include "UART.h"
#include "Switch.h"
#include "NVIC.h"

#define LED_SW P45

#define LED4 P14
#define LED3 P15
#define LED8 P20
#define LED7 P21
#define LED6 P22
#define LED5 P23
#define LED2 P26
#define LED1 P27

#define HIGH 1
#define LOW 0

/*初始化GPIO*/
void gpio_init(void)
{
	/* UART端口 */
	GPIO_InitTypeDef gpio_config;
	gpio_config.Pin = GPIO_Pin_0 | GPIO_Pin_1;
	gpio_config.Mode = GPIO_PullUp;
	GPIO_Inilize(GPIO_P3, &gpio_config);

	/* LED端口 */
	gpio_config.Pin = GPIO_Pin_4 | GPIO_Pin_5;
	GPIO_Inilize(GPIO_P1, &gpio_config);

	gpio_config.Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_Inilize(GPIO_P2, &gpio_config);
	
	/* 初始化总开关-SW */
	gpio_config.Pin = GPIO_Pin_5;
	gpio_config.Mode = GPIO_OUT_PP;
	GPIO_Inilize(GPIO_P4, &gpio_config);

}

/*初始化UART*/
void uart_init(void)
{
	// >>> 记得添加 NVIC.c, UART.c, UART_Isr.c <<<
	COMx_InitDefine COMx_InitStructure;				// 结构定义
	COMx_InitStructure.UART_Mode = UART_8bit_BRTx;	// 模式, UART_ShiftRight,UART_8bit_BRTx,UART_9bit,UART_9bit_BRTx
	COMx_InitStructure.UART_BRT_Use = BRT_Timer1;	// 选择波特率发生器, BRT_Timer1, BRT_Timer2 (注意: 串口2固定使用BRT_Timer2)
	COMx_InitStructure.UART_BaudRate = 115200ul;	// 波特率, 一般 110 ~ 115200
	COMx_InitStructure.UART_RxEnable = ENABLE;		// 接收允许,   ENABLE或DISABLE
	COMx_InitStructure.BaudRateDouble = DISABLE;	// 波特率加倍, ENABLE或DISABLE
	UART_Configuration(UART1, &COMx_InitStructure); // 初始化串口1 UART1,UART2,UART3,UART4

	NVIC_UART1_Init(ENABLE, Priority_1); // 中断使能, ENABLE/DISABLE; 优先级(低到高) Priority_0,Priority_1,Priority_2,Priority_3
	UART1_SW(UART1_SW_P30_P31);			 // 引脚选择, UART1_SW_P30_P31,UART1_SW_P36_P37,UART1_SW_P16_P17,UART1_SW_P43_P44
}

/* 初始化定时器 */
void timer_init(void)
{

	TIM_InitTypeDef TIM_InitStructure; // 结构定义
	// 定时器0做16位自动重装, 中断频率为1000HZ
	TIM_InitStructure.TIM_Mode = TIM_16BitAutoReload;			  // 指定工作模式,   TIM_16BitAutoReload,TIM_16Bit,TIM_8BitAutoReload,TIM_16BitAutoReloadNoMask
	TIM_InitStructure.TIM_ClkSource = TIM_CLOCK_1T;				  // 指定时钟源,     TIM_CLOCK_1T,TIM_CLOCK_12T,TIM_CLOCK_Ext
	TIM_InitStructure.TIM_ClkOut = DISABLE;						  // 是否输出高速脉冲, ENABLE或DISABLE
	TIM_InitStructure.TIM_Value = 65536UL - (MAIN_Fosc / 1000UL); // 初值,
	TIM_InitStructure.TIM_Run = ENABLE;							  // 是否初始化后启动定时器, ENABLE或DISABLE
	Timer_Inilize(Timer0, &TIM_InitStructure);					  // 初始化Timer0	  Timer0,Timer1,Timer2,Timer3,Timer4
	NVIC_Timer0_Init(ENABLE, Priority_0);						  // 中断使能, ENABLE/DISABLE; 优先级(低到高) Priority_0,Priority_1,Priority_2,Priority_3
}

// 定时器中断函数
// 功能: 判断是否接收到数据，此操作较耗时，需要将复杂的逻辑分离出去

u8 tmp = 0x02; // 存储串口数据的变量，0:左转向灯 1:右转向灯
void time_call(void)
{
	if (COM1.RX_TimeOut > 0)
	{
		// 超时计数
		if (--COM1.RX_TimeOut == 0)
		{
			if (COM1.RX_Cnt > 0)
			{
				// 将全局状态变量标记为对应的状态
				u8 dat = RX1_Buffer[0];
				tmp = dat;
			}
			COM1.RX_Cnt = 0;
		}
	}

}

// 右转LED控制函数
void turn_right(void)
{
	char i;
	for (i = 0; i < 8; i++)
	{
		if (i == 0)
			LED1 = 0;
		if (i == 1)
			LED2 = 0;
		if (i == 2)
			LED3 = 0;
		if (i == 3)
			LED4 = 0;
		if (i == 4)
			LED5 = 0;
		if (i == 5)
			LED6 = 0;
		if (i == 6)
			LED7 = 0;
		if (i == 7)
			LED8 = 0;
		delay_ms(100);
	}

	// 关闭所有LED
	delay_ms(200);
	LED1 = LED2 = LED3 = LED4 = LED5 = LED6 = LED7 = LED8 = 1;
}

void turn_left(void)
{
	char i;
	for (i = 7; i >= 0; i--)
	{
		if (i == 0)
			LED1 = 0;
		if (i == 1)
			LED2 = 0;
		if (i == 2)
			LED3 = 0;
		if (i == 3)
			LED4 = 0;
		if (i == 4)
			LED5 = 0;
		if (i == 5)
			LED6 = 0;
		if (i == 6)
			LED7 = 0;
		if (i == 7)
			LED8 = 0;
		delay_ms(100);
	}
	// 关闭所有LED
	delay_ms(200);
	LED1 = LED2 = LED3 = LED4 = LED5 = LED6 = LED7 = LED8 = 1;
}


int main(void)
{
	gpio_init();
	uart_init();
	timer_init();

	// 开启全局中断
	EA = 1;
	// 开启LED总开关
	LED_SW = 0;

	// 默认关闭所有LED
	LED1 = LED2 = LED3 = LED4 = LED5 = LED6 = LED7 = LED8 = 1;

	while (1)
	{
		// 处理LED的逻辑
		delay_ms(10);
		if (tmp == 0x00) // 左转
		{
			// 左流水灯
			turn_left();

			// 处理完逻辑复位状态值
			tmp = 0x02;
		}
		else if (tmp == 0x01) // 右转
		{
			// 右流水灯
			turn_right();

			// 处理完逻辑复位状态值
			tmp = 0x02;
		}
	}
	


	
}
