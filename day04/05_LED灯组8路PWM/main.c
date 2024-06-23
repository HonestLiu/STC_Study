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
	gpio_config.Mode = GPIO_OUT_PP;//推挽输出模式
	GPIO_Inilize(GPIO_P4, &gpio_config);
}

/*初始化PWM*/
#define PERIOD (MAIN_Fosc / 1000)
PWMx_Duty dutyA;
void PWM_config(void)
{
	PWMx_InitDefine PWMx_InitStructure;

	// 配置PWM1
	PWMx_InitStructure.PWM_Mode = CCMRn_PWM_MODE1;	  // 模式,		CCMRn_FREEZE,CCMRn_MATCH_VALID,CCMRn_MATCH_INVALID,CCMRn_ROLLOVER,CCMRn_FORCE_INVALID,CCMRn_FORCE_VALID,CCMRn_PWM_MODE1,CCMRn_PWM_MODE2
	PWMx_InitStructure.PWM_Duty = dutyA.PWM1_Duty;	  // PWM占空比时间, 0~Period
	PWMx_InitStructure.PWM_EnoSelect = ENO1P | ENO1N; // 输出通道选择,	ENO1P,ENO1N,ENO2P,ENO2N,ENO3P,ENO3N,ENO4P,ENO4N / ENO5P,ENO6P,ENO7P,ENO8P
	PWM_Configuration(PWM1, &PWMx_InitStructure);	  // 初始化PWM
	// 配置PWM2
	PWMx_InitStructure.PWM_Mode = CCMRn_PWM_MODE1;	  // 模式,		CCMRn_FREEZE,CCMRn_MATCH_VALID,CCMRn_MATCH_INVALID,CCMRn_ROLLOVER,CCMRn_FORCE_INVALID,CCMRn_FORCE_VALID,CCMRn_PWM_MODE1,CCMRn_PWM_MODE2
	PWMx_InitStructure.PWM_Duty = dutyA.PWM2_Duty;	  // PWM占空比时间, 0~Period
	PWMx_InitStructure.PWM_EnoSelect = ENO2P | ENO2N; // 输出通道选择,	ENO1P,ENO1N,ENO2P,ENO2N,ENO3P,ENO3N,ENO4P,ENO4N / ENO5P,ENO6P,ENO7P,ENO8P
	PWM_Configuration(PWM2, &PWMx_InitStructure);	  // 初始化PWM
	// 配置PWM3
	PWMx_InitStructure.PWM_Mode = CCMRn_PWM_MODE1;	  // 模式,		CCMRn_FREEZE,CCMRn_MATCH_VALID,CCMRn_MATCH_INVALID,CCMRn_ROLLOVER,CCMRn_FORCE_INVALID,CCMRn_FORCE_VALID,CCMRn_PWM_MODE1,CCMRn_PWM_MODE2
	PWMx_InitStructure.PWM_Duty = dutyA.PWM3_Duty;	  // PWM占空比时间, 0~Period
	PWMx_InitStructure.PWM_EnoSelect = ENO3P | ENO3N; // 输出通道选择,	ENO1P,ENO1N,ENO2P,ENO2N,ENO3P,ENO3N,ENO4P,ENO4N / ENO5P,ENO6P,ENO7P,ENO8P
	PWM_Configuration(PWM3, &PWMx_InitStructure);
	// 配置PWM4
	PWMx_InitStructure.PWM_Mode = CCMRn_PWM_MODE1;	  // 模式,		CCMRn_FREEZE,CCMRn_MATCH_VALID,CCMRn_MATCH_INVALID,CCMRn_ROLLOVER,CCMRn_FORCE_INVALID,CCMRn_FORCE_VALID,CCMRn_PWM_MODE1,CCMRn_PWM_MODE2
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
	PWM1_SW(PWM1_SW_P20_P21); // PWM1_SW_P10_P11,PWM1_SW_P20_P21,PWM1_SW_P60_P61
	PWM2_SW(PWM2_SW_P22_P23); // PWM2_SW_P12_P13,PWM2_SW_P22_P23,PWM2_SW_P62_P63
	PWM3_SW(PWM3_SW_P14_P15); // PWM3_SW_P14_P15,PWM3_SW_P24_P25,PWM3_SW_P64_P65
	PWM4_SW(PWM4_SW_P26_P27); // PWM4_SW_P16_P17,PWM4_SW_P26_P27,PWM4_SW_P66_P67,PWM4_SW_P34_P33

	// 初始化PWMA的中断
	NVIC_PWM_Init(PWMA, DISABLE, Priority_0);
}


int main()
{
		char duty_percent = 0; // 0 -> 100，标识占空比占整个周期多长时间
	  char direction = 1;//占空比每次循环相加的值
		u32 duty_value = 0;
		EAXSFR();		/* 扩展寄存器访问使能 */
	
		gpio_init();
		PWM_config();

		// 开启全局中断
		EA = 1;
		// 开启LED总开关
		LED_SW = 0;

	  // 点亮LED1,2,3
    LED1 = LED2 = LED3 = 0;
	
		//设置PWM值
		duty_value = PERIOD * duty_percent / 100;
		dutyA.PWM1_Duty = duty_value;
		dutyA.PWM2_Duty = duty_value;
		dutyA.PWM3_Duty = duty_value;
		dutyA.PWM4_Duty = duty_value; //设置占空比: 周期时间 * 占空比
		UpdatePwm(PWMA,&dutyA);//更新占空比
	
		while (1)
		{
			delay_ms(50); // 延时10ms
			
			duty_percent += direction;//修改占空比，每次加、减1
			
			//占空大于等于100，对其重新设置
			if(duty_percent >= 100)
			{
				duty_percent = 100;
				direction = -1;//设置成-1,令其每次循环都递减
			}
			else if(duty_percent <= 0)//占空比小于等于0，对其重新设置
			{
				duty_percent = 0;
				direction = 1;//设置成1，令其每次循环都递增
			}
			
			//更新设置后的占空比
			duty_value = PERIOD * duty_percent / 100;
			dutyA.PWM1_Duty = duty_value;
			dutyA.PWM2_Duty = duty_value;
			dutyA.PWM3_Duty = duty_value;
			dutyA.PWM4_Duty = duty_value; //设置占空比: 周期时间 * 占空比
			UpdatePwm(PWMA,&dutyA);//更新占空比
			
		}
			
}
