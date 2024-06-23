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

/*初始化UART*/
void uart_init(void)
{
    COMx_InitDefine		COMx_InitStructure;					//结构定义
    COMx_InitStructure.UART_Mode      = UART_8bit_BRTx;	//模式, UART_ShiftRight,UART_8bit_BRTx,UART_9bit,UART_9bit_BRTx
    COMx_InitStructure.UART_BRT_Use   = BRT_Timer1;			//选择波特率发生器, BRT_Timer1, BRT_Timer2 (注意: 串口2固定使用BRT_Timer2)
    COMx_InitStructure.UART_BaudRate  = 115200ul;			//波特率, 一般 110 ~ 115200
    COMx_InitStructure.UART_RxEnable  = ENABLE;				//接收允许,   ENABLE或DISABLE
    COMx_InitStructure.BaudRateDouble = DISABLE;			//波特率加倍, ENABLE或DISABLE
    UART_Configuration(UART1, &COMx_InitStructure);		//初始化串口1 UART1,UART2,UART3,UART4

    NVIC_UART1_Init(ENABLE,Priority_1);		//中断使能, ENABLE/DISABLE; 优先级(低到高) Priority_0,Priority_1,Priority_2,Priority_3
    UART1_SW(UART1_SW_P30_P31);		// 引脚选择, UART1_SW_P30_P31,UART1_SW_P36_P37,UART1_SW_P16_P17,UART1_SW_P43_P44
}

/* 初始化定时器 */
void timer_init(void)
{

    TIM_InitTypeDef TIM_InitStructure; // 结构定义
    // 定时器0做16位自动重装, 中断频率为1000HZ
    TIM_InitStructure.TIM_Mode = TIM_16BitAutoReload;			   // 指定工作模式,   TIM_16BitAutoReload,TIM_16Bit,TIM_8BitAutoReload,TIM_16BitAutoReloadNoMask
    TIM_InitStructure.TIM_ClkSource = TIM_CLOCK_1T;				   // 指定时钟源,     TIM_CLOCK_1T,TIM_CLOCK_12T,TIM_CLOCK_Ext
    TIM_InitStructure.TIM_ClkOut = DISABLE;						   // 是否输出高速脉冲, ENABLE或DISABLE
    TIM_InitStructure.TIM_Value = 65536UL - (MAIN_Fosc / 1000UL); // 初值,
    TIM_InitStructure.TIM_Run = ENABLE;							   // 是否初始化后启动定时器, ENABLE或DISABLE
    Timer_Inilize(Timer0, &TIM_InitStructure);					   // 初始化Timer0	  Timer0,Timer1,Timer2,Timer3,Timer4
    NVIC_Timer0_Init(ENABLE, Priority_0);						   // 中断使能, ENABLE/DISABLE; 优先级(低到高) Priority_0,Priority_1,Priority_2,Priority_3
}

// 定时器中断函数
// 功能: 设置LED的亮度
#define FREQ 100			  // 100Hz,频率，即每秒多少了周期
#define PERIOD (1000 / FREQ) // 10ms,每个周期的时间，周期 / 频率(周期需要根据实际初始化定时器的值而定)
int counter = 0;			  // 计数器，每次对LED的操作都会++
float duty = 0.2f;			  // 占空比，即1周期内高电平持续的比例

void time_call(void)
{
    if (counter < (duty * PERIOD)) // 占空比 * 周期时间 = 高电平的时间
    {
        LED1 = 0; // 点亮LED
    }
    else
    {
        LED1 = 1; // 关闭LED
    }

    if (++counter == PERIOD) // 当期操作次数等于一个周期会操作的LED次数时，会被清零
    {
        counter = 0;
    }
}




// 串口亮度调节回调函数
void on_uart_recv(void)
{
    /*
    收到0x00降低亮度
    收到0x01提高亮度
     */
    u8 dat = RX1_Buffer[0];
    if (dat == 0) // 更暗
    {
        duty -= 0.02; // 每次减少2%
        // 当它调整为0时，不继续向下减，维持原状
        if (duty < 0.0)
        {
            duty = 0.0f;
        }
    }
    else if (dat == 1)//更亮
    {
        duty += 0.02; // 每次增加2%
        // 当duty的值大于1时，维持原状不变
        if (duty > 1.0)
        {
            duty = 1.0f;
        }
    }
    printf("duty: %.2f\n", duty);
}


int main()
{
    gpio_init();
    uart_init();
    timer_init();

    // 开启全局中断
    EA = 1;
    // 开启LED总开关
    LED_SW = 0;

    // 默认关闭所有LED
    LED1 = 0;

    while (1)
    {
        delay_ms(10); // 延时10ms

        if (COM1.RX_TimeOut > 0)
        {
            // 超时计数
            if (--COM1.RX_TimeOut == 0)
            {
                if (COM1.RX_Cnt > 0)
                {
                    on_uart_recv();
                }
                COM1.RX_Cnt = 0;
            }
        }
    }
}
