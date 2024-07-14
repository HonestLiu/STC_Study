#include "Config.h"
#include "Delay.h"
#include "NVIC.h"
#include "UART.h"
#include "Switch.h"
#include "GPIO.h"
#include "Exti.h"
#include "PCF8563.h"

/**
 * @author HonestLiu
 * @note 这个程序通过操作I2C的通信协议设置了闹钟并设置了时间，闹钟将会在程序启动的6秒内触发，由于PCF8563芯片的RTC_INT引脚连接的时MCU的P37支持中断检测的引脚
 * 故当中断触发时，会触发INT3的中断回调函数，通过串口打印闹钟响应的信息
 * */

/****************  GPIO初始化函数 *****************/
void GPIO_config(void) {
    //初始化PCF8563的RTC_INT引脚 -- P37(中断引脚3)
    GPIO_InitTypeDef gpio_init_struct;
    gpio_init_struct.Mode = GPIO_PullUp;
    gpio_init_struct.Pin = GPIO_Pin_7;
    GPIO_Inilize(GPIO_P3, &gpio_init_struct);

    P3_MODE_IO_PU(GPIO_Pin_0 | GPIO_Pin_1);
}

/******************** INT配置 ********************/
void Exti_config(void) {
    EXTI_InitTypeDef Exti_InitStructure;                            //结构定义

    Exti_InitStructure.EXTI_Mode = EXT_MODE_Fall;//中断模式,只监听下降沿
    Ext_Inilize(EXT_INT3, &Exti_InitStructure);                //初始化中断3，对应引脚是p37
    NVIC_INT0_Init(ENABLE,
                   Priority_0);        //中断使能, ENABLE/DISABLE; 优先级(低到高) Priority_0,Priority_1,Priority_2,Priority_3
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

//闹钟中断触发调用函数
void PCF8536_on_alarm() {
    printf("int3_call Alarm\n");
}

//定时器中断触发调用函数
void PCF8536_on_timer() {
    printf("int3_call Timer\n");
}

int main() {
    //日期结构体，存储转换后的要设置的值
    Clock_t c;
    //闹钟结构体
    Alarm_t a;

    EAXSFR();//使能拓展寄存器

    PCF8563_init();//初始化I2C
    UART_config();
    GPIO_config();
    Exti_config();

    EA = 1;//使能中断

    /*设置RTC芯片时间*/
    //准备要设置的数据
    c.year = 2024, c.month = 6, c.day = 29, c.week = 1, c.hour = 23, c.minute = 59, c.second = 54;
    //进行时间设置
    PCF8563_set(c);

    /*4.设置闹钟时间 -- 这样当闹钟触发时INT引脚就会触发一个中断，使用支持EXIT的引脚去检测即可*/
    a.minute = 0;
    a.hour = 0;
    a.day = 30;
    a.week_day = 2;
    PCF8563_set_alarm(a);

    //TODO 如果只需要使用定时器/闹钟其一，务必使用失能函数对其进行失能，否则可能会因为芯片此前的设置而导致出错

    /*配置状态控制寄存器2启用闹钟/警报Alarm*/
    PCF8563_enable_alarm();
    //PCF8563_disable_alarm();

    /*设置RTC Timer定时器*/
    //时钟源选择64Hz,倒计时值为64
    PCF8563_set_timer(Hz64, 64);
    //清除定时器标记和使能定时器中断
    PCF8563_enable_timer();


    while (1) {
        /*循环读取RTC芯片的数据*/
        PCF8563_get(&c);

        printf("Hour:%d;Minute:%d;c.second: %d\n", (int) hour, (int) minute, (int) c.second);
        //printf("%d-%d-%d-%d-%d-%d,Week:%d\n", (int) c.year, (int) c.month, (int) c.day, (int) c.hour, (int) c.minute,
               (int) c.second, (int) c.week);

        delay_ms(250);
        delay_ms(250);
        delay_ms(250);
        delay_ms(250);
    }


}
