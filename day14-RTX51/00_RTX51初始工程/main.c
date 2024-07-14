#include "STC8H.h"
#include "RTX51TNY.H"//RTX51的系统头文件

//延时1000毫秒
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
	//不依赖GPIO头文件初始化P5管脚
	P5M1 = 0;
	P5M0 = 1;
	
	while(1){
		P53 = !P53;
		Delay1000ms();
	}
	
}