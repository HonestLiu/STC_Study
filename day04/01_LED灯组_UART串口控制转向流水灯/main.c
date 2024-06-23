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

/*��ʼ��GPIO*/
void gpio_init(void)
{
	/* UART�˿� */
	GPIO_InitTypeDef gpio_config;
	gpio_config.Pin = GPIO_Pin_0 | GPIO_Pin_1;
	gpio_config.Mode = GPIO_PullUp;
	GPIO_Inilize(GPIO_P3, &gpio_config);

	/* LED�˿� */
	gpio_config.Pin = GPIO_Pin_4 | GPIO_Pin_5;
	GPIO_Inilize(GPIO_P1, &gpio_config);

	gpio_config.Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_Inilize(GPIO_P2, &gpio_config);
	
	/* ��ʼ���ܿ���-SW */
	gpio_config.Pin = GPIO_Pin_5;
	gpio_config.Mode = GPIO_OUT_PP;
	GPIO_Inilize(GPIO_P4, &gpio_config);

}

/*��ʼ��UART*/
void uart_init(void)
{
	// >>> �ǵ���� NVIC.c, UART.c, UART_Isr.c <<<
	COMx_InitDefine COMx_InitStructure;				// �ṹ����
	COMx_InitStructure.UART_Mode = UART_8bit_BRTx;	// ģʽ, UART_ShiftRight,UART_8bit_BRTx,UART_9bit,UART_9bit_BRTx
	COMx_InitStructure.UART_BRT_Use = BRT_Timer1;	// ѡ�����ʷ�����, BRT_Timer1, BRT_Timer2 (ע��: ����2�̶�ʹ��BRT_Timer2)
	COMx_InitStructure.UART_BaudRate = 115200ul;	// ������, һ�� 110 ~ 115200
	COMx_InitStructure.UART_RxEnable = ENABLE;		// ��������,   ENABLE��DISABLE
	COMx_InitStructure.BaudRateDouble = DISABLE;	// �����ʼӱ�, ENABLE��DISABLE
	UART_Configuration(UART1, &COMx_InitStructure); // ��ʼ������1 UART1,UART2,UART3,UART4

	NVIC_UART1_Init(ENABLE, Priority_1); // �ж�ʹ��, ENABLE/DISABLE; ���ȼ�(�͵���) Priority_0,Priority_1,Priority_2,Priority_3
	UART1_SW(UART1_SW_P30_P31);			 // ����ѡ��, UART1_SW_P30_P31,UART1_SW_P36_P37,UART1_SW_P16_P17,UART1_SW_P43_P44
}

/* ��ʼ����ʱ�� */
void timer_init(void)
{

	TIM_InitTypeDef TIM_InitStructure; // �ṹ����
	// ��ʱ��0��16λ�Զ���װ, �ж�Ƶ��Ϊ1000HZ
	TIM_InitStructure.TIM_Mode = TIM_16BitAutoReload;			  // ָ������ģʽ,   TIM_16BitAutoReload,TIM_16Bit,TIM_8BitAutoReload,TIM_16BitAutoReloadNoMask
	TIM_InitStructure.TIM_ClkSource = TIM_CLOCK_1T;				  // ָ��ʱ��Դ,     TIM_CLOCK_1T,TIM_CLOCK_12T,TIM_CLOCK_Ext
	TIM_InitStructure.TIM_ClkOut = DISABLE;						  // �Ƿ������������, ENABLE��DISABLE
	TIM_InitStructure.TIM_Value = 65536UL - (MAIN_Fosc / 1000UL); // ��ֵ,
	TIM_InitStructure.TIM_Run = ENABLE;							  // �Ƿ��ʼ����������ʱ��, ENABLE��DISABLE
	Timer_Inilize(Timer0, &TIM_InitStructure);					  // ��ʼ��Timer0	  Timer0,Timer1,Timer2,Timer3,Timer4
	NVIC_Timer0_Init(ENABLE, Priority_0);						  // �ж�ʹ��, ENABLE/DISABLE; ���ȼ�(�͵���) Priority_0,Priority_1,Priority_2,Priority_3
}

// ��ʱ���жϺ���
// ����: �ж��Ƿ���յ����ݣ��˲����Ϻ�ʱ����Ҫ�����ӵ��߼������ȥ

u8 tmp = 0x02; // �洢�������ݵı�����0:��ת��� 1:��ת���
void time_call(void)
{
	if (COM1.RX_TimeOut > 0)
	{
		// ��ʱ����
		if (--COM1.RX_TimeOut == 0)
		{
			if (COM1.RX_Cnt > 0)
			{
				// ��ȫ��״̬�������Ϊ��Ӧ��״̬
				u8 dat = RX1_Buffer[0];
				tmp = dat;
			}
			COM1.RX_Cnt = 0;
		}
	}

}

// ��תLED���ƺ���
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

	// �ر�����LED
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
	// �ر�����LED
	delay_ms(200);
	LED1 = LED2 = LED3 = LED4 = LED5 = LED6 = LED7 = LED8 = 1;
}


int main(void)
{
	gpio_init();
	uart_init();
	timer_init();

	// ����ȫ���ж�
	EA = 1;
	// ����LED�ܿ���
	LED_SW = 0;

	// Ĭ�Ϲر�����LED
	LED1 = LED2 = LED3 = LED4 = LED5 = LED6 = LED7 = LED8 = 1;

	while (1)
	{
		// ����LED���߼�
		delay_ms(10);
		if (tmp == 0x00) // ��ת
		{
			// ����ˮ��
			turn_left();

			// �������߼���λ״ֵ̬
			tmp = 0x02;
		}
		else if (tmp == 0x01) // ��ת
		{
			// ����ˮ��
			turn_right();

			// �������߼���λ״ֵ̬
			tmp = 0x02;
		}
	}
	


	
}
