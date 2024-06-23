#include "Config.h"
#include "GPIO.h"
#include "Delay.h"
#include "Timer.h"
#include "UART.h"
#include "Switch.h"
#include "NVIC.h"
#include "STC8H_PWM.h"

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
	gpio_config.Mode = GPIO_OUT_PP; // 推挽输出模式
	GPIO_Inilize(GPIO_P4, &gpio_config);
}

/*初始化PWM*/
#define PERIOD (MAIN_Fosc / 1000)
PWMx_Duty dutyA;
void PWM_config(void)
{
	PWMx_InitDefine PWMx_InitStructure;

	// 配置PWM4
	PWMx_InitStructure.PWM_Mode = CCMRn_PWM_MODE2;	  // 模式,		CCMRn_FREEZE,CCMRn_MATCH_VALID,CCMRn_MATCH_INVALID,CCMRn_ROLLOVER,CCMRn_FORCE_INVALID,CCMRn_FORCE_VALID,CCMRn_PWM_MODE1,CCMRn_PWM_MODE2
	PWMx_InitStructure.PWM_Duty = dutyA.PWM4_Duty;	  // PWM占空比时间, 0~Period
	PWMx_InitStructure.PWM_EnoSelect = ENO4P | ENO4N; // 输出通道选择,	ENO1P,ENO1N,ENO2P,ENO2N,ENO3P,ENO3N,ENO4P,ENO4N / ENO5P,ENO6P,ENO7P,ENO8P
	PWM_Configuration(PWM4, &PWMx_InitStructure);

	// 配置PWMA
	PWMx_InitStructure.PWM_Period = PERIOD - 1;	   // 周期时间,   0~65535
	PWMx_InitStructure.PWM_DeadTime = 0;		   // 死区发生器设置, 0~255
	PWMx_InitStructure.PWM_MainOutEnable = ENABLE; // 主输出使能, ENABLE,DISABLE
	PWMx_InitStructure.PWM_CEN_Enable = ENABLE;	   // 使能计数器, ENABLE,DISABLE
	PWM_Configuration(PWMA, &PWMx_InitStructure);  // 初始化PWM通用寄存器,  PWMA,PWMB

	// 切换PWM通道
	PWM4_SW(PWM4_SW_P26_P27); // PWM4_SW_P16_P17,PWM4_SW_P26_P27,PWM4_SW_P66_P67,PWM4_SW_P34_P33

	// 初始化PWMA的中断
	NVIC_PWM_Init(PWMA, DISABLE, Priority_0);
}

/*初始化UART1*/
void UART_config(void)
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

// 在接收数据回调函数内控制PWM的占空比
char duty_percent = 0; // 0 -> 100，标识占空比占整个周期多长时间
void on_receive()
{
	u8 dat = RX1_Buffer[0];
	// 接收到0，降低占空比
	if (dat == 0x00)
	{
		duty_percent -= 1;
		printf("duty_percent:%d\n", (int)duty_percent);
		// 如果占空比已减法至0，维持0即可，不能让其变为负数
		if (duty_percent < 0)
		{
			duty_percent = 0;
		}
	}
	else if (dat == 0x01) // 接收到1，提高占空比
	{
		duty_percent += 1;
		printf("duty_percent:%d\n", (int)duty_percent);
		// 如果占空比已增至100，维持100即可，不能让其超过100
		if (duty_percent > 100)
		{
			duty_percent = 100;
		}
	}
	// 更新设置后的占空比
	dutyA.PWM4_Duty = PERIOD * duty_percent / 100; // 设置占空比: 周期时间 * 占空比
	UpdatePwm(PWM4, &dutyA);
}

int main()
{
	char i = 0;
	EAXSFR(); /* 扩展寄存器访问使能 */

	gpio_init();
	PWM_config();
	UART_config();

	// 开启全局中断
	EA = 1;
	// 开启LED总开关
	LED_SW = 0;

	// 点亮LED1,2,3
	LED1 = LED2 = LED3 = 0;

	while (1)
	{

		if (COM1.RX_TimeOut > 0)
		{
			// 超时计数
			if (--COM1.RX_TimeOut == 0)
			{
				if (COM1.RX_Cnt > 0)
				{
					// 这里处理收到的数据，做具体的逻辑，可以调用自己的on_uart1_recv
					for (i = 0; i < COM1.RX_Cnt; i++)
					{
						// RX1_Buffer[i]存的是接收的每个字节，写出用 TX1_write2buff
						on_receive();
					}
				}
				COM1.RX_Cnt = 0;
			}
		}

		// 不要处理的太快
		delay_ms(10);
	}
}
