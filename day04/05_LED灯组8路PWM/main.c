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
	gpio_config.Mode = GPIO_OUT_PP;//�������ģʽ
	GPIO_Inilize(GPIO_P4, &gpio_config);
}

/*��ʼ��PWM*/
#define PERIOD (MAIN_Fosc / 1000)
PWMx_Duty dutyA;
void PWM_config(void)
{
	PWMx_InitDefine PWMx_InitStructure;

	// ����PWM1
	PWMx_InitStructure.PWM_Mode = CCMRn_PWM_MODE1;	  // ģʽ,		CCMRn_FREEZE,CCMRn_MATCH_VALID,CCMRn_MATCH_INVALID,CCMRn_ROLLOVER,CCMRn_FORCE_INVALID,CCMRn_FORCE_VALID,CCMRn_PWM_MODE1,CCMRn_PWM_MODE2
	PWMx_InitStructure.PWM_Duty = dutyA.PWM1_Duty;	  // PWMռ�ձ�ʱ��, 0~Period
	PWMx_InitStructure.PWM_EnoSelect = ENO1P | ENO1N; // ���ͨ��ѡ��,	ENO1P,ENO1N,ENO2P,ENO2N,ENO3P,ENO3N,ENO4P,ENO4N / ENO5P,ENO6P,ENO7P,ENO8P
	PWM_Configuration(PWM1, &PWMx_InitStructure);	  // ��ʼ��PWM
	// ����PWM2
	PWMx_InitStructure.PWM_Mode = CCMRn_PWM_MODE1;	  // ģʽ,		CCMRn_FREEZE,CCMRn_MATCH_VALID,CCMRn_MATCH_INVALID,CCMRn_ROLLOVER,CCMRn_FORCE_INVALID,CCMRn_FORCE_VALID,CCMRn_PWM_MODE1,CCMRn_PWM_MODE2
	PWMx_InitStructure.PWM_Duty = dutyA.PWM2_Duty;	  // PWMռ�ձ�ʱ��, 0~Period
	PWMx_InitStructure.PWM_EnoSelect = ENO2P | ENO2N; // ���ͨ��ѡ��,	ENO1P,ENO1N,ENO2P,ENO2N,ENO3P,ENO3N,ENO4P,ENO4N / ENO5P,ENO6P,ENO7P,ENO8P
	PWM_Configuration(PWM2, &PWMx_InitStructure);	  // ��ʼ��PWM
	// ����PWM3
	PWMx_InitStructure.PWM_Mode = CCMRn_PWM_MODE1;	  // ģʽ,		CCMRn_FREEZE,CCMRn_MATCH_VALID,CCMRn_MATCH_INVALID,CCMRn_ROLLOVER,CCMRn_FORCE_INVALID,CCMRn_FORCE_VALID,CCMRn_PWM_MODE1,CCMRn_PWM_MODE2
	PWMx_InitStructure.PWM_Duty = dutyA.PWM3_Duty;	  // PWMռ�ձ�ʱ��, 0~Period
	PWMx_InitStructure.PWM_EnoSelect = ENO3P | ENO3N; // ���ͨ��ѡ��,	ENO1P,ENO1N,ENO2P,ENO2N,ENO3P,ENO3N,ENO4P,ENO4N / ENO5P,ENO6P,ENO7P,ENO8P
	PWM_Configuration(PWM3, &PWMx_InitStructure);
	// ����PWM4
	PWMx_InitStructure.PWM_Mode = CCMRn_PWM_MODE1;	  // ģʽ,		CCMRn_FREEZE,CCMRn_MATCH_VALID,CCMRn_MATCH_INVALID,CCMRn_ROLLOVER,CCMRn_FORCE_INVALID,CCMRn_FORCE_VALID,CCMRn_PWM_MODE1,CCMRn_PWM_MODE2
	PWMx_InitStructure.PWM_Duty = dutyA.PWM4_Duty;	  // PWMռ�ձ�ʱ��, 0~Period
	PWMx_InitStructure.PWM_EnoSelect = ENO4P | ENO4N; // ���ͨ��ѡ��,	ENO1P,ENO1N,ENO2P,ENO2N,ENO3P,ENO3N,ENO4P,ENO4N / ENO5P,ENO6P,ENO7P,ENO8P
	PWM_Configuration(PWM4, &PWMx_InitStructure);

	// ����PWMA
	PWMx_InitStructure.PWM_Period = PERIOD - 1;	   // ����ʱ��,   0~65535
	PWMx_InitStructure.PWM_DeadTime = 0;		   // ��������������, 0~255
	PWMx_InitStructure.PWM_MainOutEnable = ENABLE; // �����ʹ��, ENABLE,DISABLE
	PWMx_InitStructure.PWM_CEN_Enable = ENABLE;	   // ʹ�ܼ�����, ENABLE,DISABLE
	PWM_Configuration(PWMA, &PWMx_InitStructure);  // ��ʼ��PWMͨ�üĴ���,  PWMA,PWMB

	// �л�PWMͨ��
	PWM1_SW(PWM1_SW_P20_P21); // PWM1_SW_P10_P11,PWM1_SW_P20_P21,PWM1_SW_P60_P61
	PWM2_SW(PWM2_SW_P22_P23); // PWM2_SW_P12_P13,PWM2_SW_P22_P23,PWM2_SW_P62_P63
	PWM3_SW(PWM3_SW_P14_P15); // PWM3_SW_P14_P15,PWM3_SW_P24_P25,PWM3_SW_P64_P65
	PWM4_SW(PWM4_SW_P26_P27); // PWM4_SW_P16_P17,PWM4_SW_P26_P27,PWM4_SW_P66_P67,PWM4_SW_P34_P33

	// ��ʼ��PWMA���ж�
	NVIC_PWM_Init(PWMA, DISABLE, Priority_0);
}


int main()
{
		char duty_percent = 0; // 0 -> 100����ʶռ�ձ�ռ�������ڶ೤ʱ��
	  char direction = 1;//ռ�ձ�ÿ��ѭ����ӵ�ֵ
		u32 duty_value = 0;
		EAXSFR();		/* ��չ�Ĵ�������ʹ�� */
	
		gpio_init();
		PWM_config();

		// ����ȫ���ж�
		EA = 1;
		// ����LED�ܿ���
		LED_SW = 0;

	  // ����LED1,2,3
    LED1 = LED2 = LED3 = 0;
	
		//����PWMֵ
		duty_value = PERIOD * duty_percent / 100;
		dutyA.PWM1_Duty = duty_value;
		dutyA.PWM2_Duty = duty_value;
		dutyA.PWM3_Duty = duty_value;
		dutyA.PWM4_Duty = duty_value; //����ռ�ձ�: ����ʱ�� * ռ�ձ�
		UpdatePwm(PWMA,&dutyA);//����ռ�ձ�
	
		while (1)
		{
			delay_ms(50); // ��ʱ10ms
			
			duty_percent += direction;//�޸�ռ�ձȣ�ÿ�μӡ���1
			
			//ռ�մ��ڵ���100��������������
			if(duty_percent >= 100)
			{
				duty_percent = 100;
				direction = -1;//���ó�-1,����ÿ��ѭ�����ݼ�
			}
			else if(duty_percent <= 0)//ռ�ձ�С�ڵ���0��������������
			{
				duty_percent = 0;
				direction = 1;//���ó�1������ÿ��ѭ��������
			}
			
			//�������ú��ռ�ձ�
			duty_value = PERIOD * duty_percent / 100;
			dutyA.PWM1_Duty = duty_value;
			dutyA.PWM2_Duty = duty_value;
			dutyA.PWM3_Duty = duty_value;
			dutyA.PWM4_Duty = duty_value; //����ռ�ձ�: ����ʱ�� * ռ�ձ�
			UpdatePwm(PWMA,&dutyA);//����ռ�ձ�
			
		}
			
}
