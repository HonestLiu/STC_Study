#include "STC8H.h"
#include "RTX51TNY.H"//RTX51��ϵͳͷ�ļ�

//��ʱ1000����
void Delay1000ms(void)	//@11.0592MHz
{
	unsigned char data i, j, k;

	i = 57;
	j = 27;
	k = 112;
	do
	{
		do
		{
			while (--k);
		} while (--j);
	} while (--i);
}


void start() _task_ 0{
	//������GPIOͷ�ļ���ʼ��P5�ܽ�
	P5M1 = 0;
	P5M0 = 1;
	
	while(1){
		P53 = !P53;
		Delay1000ms();
	}
	
}