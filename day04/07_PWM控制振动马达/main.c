#include "Config.h"
#include "GPIO.h"
#include "Delay.h"
#include "NVIC.h"
#include "Switch.h"
#include "STC8H_PWM.h"

/**
* ˵��: ͨ��PWM����PNP�����ܼ�ӿ����񶯵����ͨ������ռ�ձȣ��ı���������ǿ��
* 1: ��ͨ�������  0:�պϣ�����ر�
*/

#define MOTOR P01

//��ʼ��P01����(���������ܵ�����)
void gpio_config(void)
{
    P0_MODE_OUT_PP(GPIO_Pin_1);//����P01,�����������֤���㹻�ĵ�ƽ��ͨ������
}

//��ʼ��PWM��P01���Ź�����PWMB�����PWM6
#define PERIOD (MAIN_Fosc / 1000) //��������
PWMx_Duty dutyB;
void PWM_config(void)
{
    PWMx_InitDefine		PWMx_InitStructure;

    // ����PWM6
    PWMx_InitStructure.PWM_Mode    		= CCMRn_PWM_MODE1;	//ģʽ,		CCMRn_FREEZE,CCMRn_MATCH_VALID,CCMRn_MATCH_INVALID,CCMRn_ROLLOVER,CCMRn_FORCE_INVALID,CCMRn_FORCE_VALID,CCMRn_PWM_MODE1,CCMRn_PWM_MODE2
    PWMx_InitStructure.PWM_Duty    		= dutyB.PWM6_Duty;	//PWMռ�ձ�ʱ��, 0~Period
    PWMx_InitStructure.PWM_EnoSelect    = ENO6P;			//���ͨ��ѡ��,	ENO1P,ENO1N,ENO2P,ENO2N,ENO3P,ENO3N,ENO4P,ENO4N / ENO5P,ENO6P,ENO7P,ENO8P
    PWM_Configuration(PWM6, &PWMx_InitStructure);			//��ʼ��PWM,  PWMA,PWMB

    // ����PWMB
    PWMx_InitStructure.PWM_Period   = PERIOD - 1;			//����ʱ��,   0~65535
    PWMx_InitStructure.PWM_DeadTime = 0;					//��������������, 0~255
    PWMx_InitStructure.PWM_MainOutEnable= ENABLE;			//�����ʹ��, ENABLE,DISABLE
    PWMx_InitStructure.PWM_CEN_Enable   = ENABLE;			//ʹ�ܼ�����, ENABLE,DISABLE
    PWM_Configuration(PWMB, &PWMx_InitStructure);			//��ʼ��PWMͨ�üĴ���,  PWMA,PWMB

    // �л�PWMͨ��
    PWM6_SW(PWM6_SW_P01);					//PWM6_SW_P21,PWM6_SW_P54,PWM6_SW_P01,PWM6_SW_P75

    // ��ʼ��PWMB���ж�
    NVIC_PWM_Init(PWMB,DISABLE,Priority_0);
}

int main()
{
    char duty_percent = 0; // 0 -> 100����ʶռ�ձ�ռ�������ڶ೤ʱ��

    EAXSFR();		/* ��չ�Ĵ�������ʹ�� */

    PWM_config();
    gpio_config();

    EA = 1;//ʹ��ȫ���ж�
	
    dutyB.PWM6_Duty = 0;
    UpdatePwm(PWM6,&dutyB);

    while(1)
    {
        delay_ms(10);
        duty_percent++;
        if(duty_percent > 100)
        {
            duty_percent = 0;
        }
        dutyB.PWM6_Duty = PERIOD * dutyB.PWM6_Duty / 100;
        UpdatePwm(PWM6,&dutyB);

    }
}

