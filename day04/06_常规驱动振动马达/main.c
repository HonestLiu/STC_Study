#include "Config.h"
#include "GPIO.h"
#include "Delay.h"

/**
* 说明: 用于驱动PNP三级管间接控制振动电机
* 1: 导通，电机振动  0:闭合，电机关闭
*/

#define MOTOR P01

//初始化P01引脚(连接三极管的引脚)
void gpio_config(void)
{
	P0_MODE_OUT_PP(GPIO_Pin_1);//设置P01,推挽输出，保证有足够的电平导通三极管
}

int main()
{
	gpio_config();
	MOTOR = 1;//先导通三极管
	while(1)
	{
		delay_ms(250);
		delay_ms(250);
		MOTOR = 0;
		delay_ms(250);
		delay_ms(250);
		MOTOR = 1;
	}
}

