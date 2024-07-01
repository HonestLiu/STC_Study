#include "Config.h"
#include "GPIO.h"
#include "Delay.h"
#include "NVIC.h"
#include "UART.h"
#include "Switch.h"
#include "I2C.h"

/****************  I2C GPIO初始化函数 *****************/
void GPIO_config() {
    GPIO_InitTypeDef gpio_init_struct;
    gpio_init_struct.Mode = GPIO_OUT_OD;//开漏输出
    gpio_init_struct.Pin = GPIO_Pin_2 | GPIO_Pin_3;//I2C总线所使用的两个IO
    GPIO_Inilize(GPIO_P3, &gpio_init_struct);

    P3_MODE_IO_PU(GPIO_Pin_0 | GPIO_Pin_1);

}

/****************  UART初始化函数 *****************/
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

/****************  I2C初始化函数 *****************/
void I2C_config(void) {
    I2C_InitTypeDef I2C_InitStructure;

    I2C_InitStructure.I2C_Mode = I2C_Mode_Master;    //主从选择   I2C_Mode_Master, I2C_Mode_Slave
    I2C_InitStructure.I2C_Enable = ENABLE;            //I2C功能使能,   ENABLE, DISABLE
    I2C_InitStructure.I2C_MS_WDTA = DISABLE;            //主机使能自动发送,  ENABLE, DISABLE
    I2C_InitStructure.I2C_Speed = 13;                //总线速度=Fosc/2/(Speed*2+4),      0~63
    I2C_Init(&I2C_InitStructure);
    NVIC_I2C_Init(I2C_Mode_Master, DISABLE,
                  Priority_0);    //主从模式, I2C_Mode_Master, I2C_Mode_Slave; 中断使能, ENABLE/DISABLE; 优先级(低到高) Priority_0,Priority_1,Priority_2,Priority_3

    I2C_SW(I2C_P33_P32);                    //I2C_P14_P15,I2C_P24_P25,I2C_P33_P32
}

//要读的位数,其会自动进位，即读完秒，会自动移动到秒和时的地方去获取
#define NUMBER 7

int main() {
    // 设备地址 read:A3h write A2h(从PCF8563英文文档中获得)
    u8 dev_addr = 0x51 << 1; //写地址,0x51是PCF8563的基地址，将它左移相当于让其右侧加个0，最后结果就是A2h
    // 秒存储地址 0x02; //(从PCF8563英文文档中获得)
    u8 mem_addr = 0x02;
    //用于接收从机传来的数据
    u8 p[NUMBER];

    //存储转换后的值
    u8 second, minute, hour, day, week, month, C;
    u16 year;

    EAXSFR();//使能拓展寄存器

    GPIO_config();
    UART_config();
    I2C_config();

    EA = 1;//使能中断

    while (1) {
        //通过I2C读取RTC时钟芯片的秒数据
        I2C_ReadNbyte(dev_addr, mem_addr, &p, NUMBER);

        /**
         * 将 秒 转成十进制(BSD秒to十进制秒)
         * 过程:
         * 1.先将高位移动4位到低位，再&0x7F取出高三位，即十位
         * 2.再直接对接收到的数据&0x0F,将低4位取出来，即个位
         * 3.两则相加，得到的就是需要秒数的十进制数了
         * */

        second = ((p[0] >> 4) & 0x07) * 10 + (p[0] & 0x0F);//秒，高4位全取，低4位全取
        minute = ((p[1] >> 4) & 0x07) * 10 + (p[1] & 0x0F);//分，高4位全取，低4位全取
        hour = ((p[2] >> 4) & 0x03) * 10 + (p[2] & 0x0F);//时，高4位需要取两位，低4位全取
        day = ((p[3] >> 4) & 0x03) * 10 + (p[3] & 0x0F);//天，高4位需要取两位，低4位全取
        week = p[4] & 0x07;//周,只取低3位
        month = ((p[5] >> 4) & 0x01) * 10 + (p[5] & 0x0F);//月,高4位取1位，低4位全取
        C = p[5] >> 7;//世纪，只取最高位。 --> 0表示20xx年 1表示21xx年
        year = ((p[6] >> 4) & 0x0F) * 10 + (p[6] & 0x0F);//年，全都有;  --> 其范围是0~99

        /*根据BSD获取的年的范围是0~99，需要得到我们常见的"年"，需要将BSD得到的year与C所对应的值相加*/
        year += (C == 0 ? 2000 : 2100);

        //printf("Hour:%d;Minute:%d;Second: %d\n", (int) hour, (int) minute, (int) second);
        printf("%d-%d-%d-%d-%d-%d,Week:%d\n",(int)year,(int)month,(int)day,(int)hour,(int)minute,(int)second,(int)week);

        delay_ms(250);
        delay_ms(250);
        delay_ms(250);
        delay_ms(250);
    }


}
