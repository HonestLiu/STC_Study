#include "Config.h"
#include "GPIO.h"
#include "Delay.h"

/**
* ˵��: ��������PNP�����ܼ�ӿ����񶯵��
* 1: ��ͨ�������  0:�պϣ�����ر�
*/

#define MOTOR P01

//��ʼ��P01����(���������ܵ�����)
void gpio_config(void)
{
	P0_MODE_OUT_PP(GPIO_Pin_1);//����P01,�����������֤���㹻�ĵ�ƽ��ͨ������
}

int main()
{
	gpio_config();
	MOTOR = 1;//�ȵ�ͨ������
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

