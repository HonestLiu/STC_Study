#include "Config.h"
#include "GPIO.h"
#include "UART.h"
#include "EEPROM.h"
#include "Delay.h"
#include "NVIC.h"
#include "Switch.h"
#include <string.h>

/******************* IO配置函数 *******************/
void GPIO_config(void) {
    GPIO_InitTypeDef GPIO_InitStructure;        //结构定义

    GPIO_InitStructure.Pin = GPIO_Pin_0 | GPIO_Pin_1;        //指定要初始化的IO, GPIO_Pin_0 ~ GPIO_Pin_7
    GPIO_InitStructure.Mode = GPIO_PullUp;    //指定IO的输入或输出方式,GPIO_PullUp,GPIO_HighZ,GPIO_OUT_OD,GPIO_OUT_PP
    GPIO_Inilize(GPIO_P3, &GPIO_InitStructure);    //初始化
}

//初始化UART
void UART_config(void) {
    // >>> 记得添加 NVIC.c, UART.c, UART_Isr.c <<<
    COMx_InitDefine COMx_InitStructure;                    //结构定义
    COMx_InitStructure.UART_Mode = UART_8bit_BRTx;    //模式, UART_ShiftRight,UART_8bit_BRTx,UART_9bit,UART_9bit_BRTx
    COMx_InitStructure.UART_BRT_Use = BRT_Timer1;            //选择波特率发生器, BRT_Timer1, BRT_Timer2 (注意: 串口2固定使用BRT_Timer2)
    COMx_InitStructure.UART_BaudRate = 115200ul;            //波特率, 一般 110 ~ 115200
    COMx_InitStructure.UART_RxEnable = ENABLE;                //接收允许,   ENABLE或DISABLE
    COMx_InitStructure.BaudRateDouble = DISABLE;            //波特率加倍, ENABLE或DISABLE
    UART_Configuration(UART1, &COMx_InitStructure);        //初始化串口1 UART1,UART2,UART3,UART4

    NVIC_UART1_Init(ENABLE,
                    Priority_1);        //中断使能, ENABLE/DISABLE; 优先级(低到高) Priority_0,Priority_1,Priority_2,Priority_3
    UART1_SW(UART1_SW_P30_P31);        // 引脚选择, UART1_SW_P30_P31,UART1_SW_P36_P37,UART1_SW_P16_P17,UART1_SW_P43_P44
}

#define     Max_Length          100      //读写EEPROM缓冲长度
u8 xdata tmp[Max_Length];        //EEPROM操作缓冲

int main() {
    //要操作扇区的起始地址
    u16 addr_sector = 0x0040;
    //要存储的字符串
    char *str = "Doraemon";
    u16 str_len = strlen(str);

    EAXSFR();		/* 扩展寄存器访问使能 */
    GPIO_config();
    UART_config();
    EA = 1;//失能中断


    //擦除扇区，一次性擦除一个扇区512字节，从0x0000开始到0x01FF
    EEPROM_SectorErase(addr_sector);
    //写入数据，String\int\long\float
    EEPROM_write_n(addr_sector, str, str_len);
    //从扇区读取数据
    EEPROM_read_n(addr_sector, tmp, str_len);


    printf("写入的字符串:%s\n", str);
    //为存储读取字符串的数组添加字符串结束符，避免出现错误
    tmp[str_len] = '\0';
    printf("读到的字符串为:%s\n", tmp);

    while (1) {
        printf("读到的字符串为:%s\n", tmp);
        delay_ms(250);
        delay_ms(250);
        delay_ms(250);
        delay_ms(250);
    }
}