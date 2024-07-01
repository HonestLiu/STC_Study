#include "Config.h"
#include "GPIO.h"
#include "Delay.h"
#include "NVIC.h"
#include "Switch.h"
#include "STC8H_PWM.h"

/**
* 说明: 通过PWM驱动PNP三级管间接控制振动电机，通过调整占空比，改变振动马达的振动强度
* 1: 导通，电机振动  0:闭合，电机关闭
*/

#define MOTOR P01

//初始化P01引脚(连接三极管的引脚)
void gpio_config(void)
{
    P0_MODE_OUT_PP(GPIO_Pin_1);//设置P01,推挽输出，保证有足够的电平导通三极管
}

//初始化PWM，P01引脚归属于PWMB分类的PWM6
#define PERIOD (MAIN_Fosc / 1000) //机器周期
PWMx_Duty dutyB;
void PWM_config(void)
{
    PWMx_InitDefine		PWMx_InitStructure;

    // 配置PWM6
    PWMx_InitStructure.PWM_Mode    		= CCMRn_PWM_MODE1;	//模式,		CCMRn_FREEZE,CCMRn_MATCH_VALID,CCMRn_MATCH_INVALID,CCMRn_ROLLOVER,CCMRn_FORCE_INVALID,CCMRn_FORCE_VALID,CCMRn_PWM_MODE1,CCMRn_PWM_MODE2
    PWMx_InitStructure.PWM_Duty    		= dutyB.PWM6_Duty;	//PWM占空比时间, 0~Period
    PWMx_InitStructure.PWM_EnoSelect    = ENO6P;			//输出通道选择,	ENO1P,ENO1N,ENO2P,ENO2N,ENO3P,ENO3N,ENO4P,ENO4N / ENO5P,ENO6P,ENO7P,ENO8P
    PWM_Configuration(PWM6, &PWMx_InitStructure);			//初始化PWM,  PWMA,PWMB

    // 配置PWMB
    PWMx_InitStructure.PWM_Period   = PERIOD - 1;			//周期时间,   0~65535
    PWMx_InitStructure.PWM_DeadTime = 0;					//死区发生器设置, 0~255
    PWMx_InitStructure.PWM_MainOutEnable= ENABLE;			//主输出使能, ENABLE,DISABLE
    PWMx_InitStructure.PWM_CEN_Enable   = ENABLE;			//使能计数器, ENABLE,DISABLE
    PWM_Configuration(PWMB, &PWMx_InitStructure);			//初始化PWM通用寄存器,  PWMA,PWMB

    // 切换PWM通道
    PWM6_SW(PWM6_SW_P01);					//PWM6_SW_P21,PWM6_SW_P54,PWM6_SW_P01,PWM6_SW_P75

    // 初始化PWMB的中断
    NVIC_PWM_Init(PWMB,DISABLE,Priority_0);
}

int main()
{
    char duty_percent = 0; // 0 -> 100，标识占空比占整个周期多长时间

    EAXSFR();		/* 扩展寄存器访问使能 */

    PWM_config();
    gpio_config();

    EA = 1;//使能全局中断
	
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

