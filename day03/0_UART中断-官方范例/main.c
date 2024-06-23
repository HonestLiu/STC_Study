/**
* @author HonestLiu
* @note 启动时会点亮P53端口上的LED(进入while循环后就会灭)，而后就会进入while循环只需监听用户输入，当发来信息时RI位会置1，就会进入if语句块点亮LED，后继又会因为通过
* SBUF发送数据TI位置1，被关闭
*
*/

#include "reg51.h"
#include "intrins.h"

/*设置波特率的*/
sfr     T2L     =   0xd7;
sfr     T2H     =   0xd6;
sfr     AUXR    =   0x8e;

sfr     P0M1    =   0x93;
sfr     P0M0    =   0x94;
sfr     P1M1    =   0x91;
sfr     P1M0    =   0x92;
sfr     P2M1    =   0x95;
sfr     P2M0    =   0x96;
sfr     P3M1    =   0xb1;
sfr     P3M0    =   0xb2;
sfr     P4M1    =   0xb3;
sfr     P4M0    =   0xb4;
sfr     P5M1    =   0xc9;
sfr     P5M0    =   0xca;
sfr		P5		= 	0xc8;//P5寄存器基地址

sbit    P10     =   P1^0;
sbit    P11     =   P1^1;
sbit	P53		= 	P5^3;//P5寄存器从左往右数第3位，对应的就是P53

void UART1_Isr() interrupt 4
{
    if (TI)//如果寄存器TI触发中断，关闭灯
    {
		// TI位为发送数据后收到置1的
        TI = 0;                                 //清中断标志
        P53 = 0;                             	//关闭灯
    }
    if (RI)//如果接收寄存器RI触发了中断，打开灯
    {
		//RI位是接收到数据系统自动置1的，这里需要手动清除中断标记
        RI = 0;                                 //清中断标志
        P53 = 1;                             //打开灯
    }
}

/*延时函数-ISP生成*/
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


void main()
{
	//初始化端口为双向输出模式
    P0M0 = 0x00;
    P0M1 = 0x00;
    P1M0 = 0x00;
    P1M1 = 0x00;
    P2M0 = 0x00;
    P2M1 = 0x00;
    P3M0 = 0x00;
    P3M1 = 0x00;
    P4M0 = 0x00;
    P4M1 = 0x00;
    P5M0 = 0x00;
    P5M1 = 0x00;

    SCON = 0x50;								//SM1 设置模式1 及 REN 串口接收控制位 (0x40 | 0x10) 将前面两个位都置1了
    T2L = 0xe8;                                 //65536-11059200/115200/4=0FFE8H，计数值
    T2H = 0xff;
    AUXR = 0x15;                                //启动定时器
    ES = 1;                                     //使能串口1的中断
    EA = 1;										//使能中断总开关

	P53 = 1;									//开始先把灯打开(很快就会被熄灭)
	
    while (1)
	{
		//休眠1S
		Delay1000ms();
		
		//发送测试数据0x23->"#"
		SBUF = 0x23;
		
		//将TI位置1，标记发送数据
		TI = 1;
	}
}

