#include "Config.h"
#include "GPIO.h"

void gpio_config()
{
	GPIO_InitTypeDef gpio_struct;
	gpio_struct.Pin = GPIO_Pin_3;
	gpio_struct.Mode = GPIO_OUT_PP;
	
	GPIO_Inilize(GPIO_P5,&gpio_struct);
}

int main()
{
	gpio_config();
	
	while(1)
	{
		P53 = 1;
		NOP1();
		P53 = 0;
		//NOP1();	
	}
	
}
