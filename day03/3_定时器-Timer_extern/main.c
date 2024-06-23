/**
 * @author HonestLiu
 * @note 通过定时器每1ms调整一次LED灯的亮灭,定时器回调函数逻辑在"Timer_lsr.c"的Timer0_ISR_Handler中进行定义
 **/
#include "Config.h"
#include "GPIO.h"
#include "Timer.h"
#include "NVIC.h"

/* LED引脚初始化函数->P53 */
void gpio_config(void)
{
    GPIO_InitTypeDef gpio_struct;
    gpio_struct.Pin = GPIO_Pin_3;
    gpio_struct.Mode = GPIO_PullUp; // 上拉准双向口
    GPIO_Inilize(GPIO_P5, &gpio_struct);
}

/* 定时器初始化函数-Timer0 */
void timer_init(void)
{
    TIM_InitTypeDef tim_struct;
    tim_struct.TIM_Mode = TIM_16BitAutoReload;			   // 16位自动重装载模式
    tim_struct.TIM_ClkSource = TIM_CLOCK_1T;			   // 时钟源为1T
    tim_struct.TIM_ClkOut = DISABLE;					   // 不输出高数脉冲
    tim_struct.TIM_Value = 65536UL - (MAIN_Fosc / 1000UL); // 初值
    tim_struct.TIM_Run = ENABLE;						   // 是否初始化后启动定时器
    Timer_Inilize(Timer0, &tim_struct);

    NVIC_Timer0_Init(ENABLE, Priority_0); // 中断使能, ENABLE/DISABLE; 优先级(低到高) Priority_0,Priority_1,Priority_2,Priority_3
}

/*定时器回调逻辑->每次定时器溢出都会调用*/
int count = 0;
void tim0_call(void)
{
    /*让P53灯每500ms切换一次亮灭*/
    count++;
    if(count == 500)//回调函数每1ms被调用一次，使用变量计数，当变量满500时切换LED引脚状态
    {
        P53 = ~P53;
    }
}

int main()
{
    gpio_config();
    timer_init();
    EA = 1;	 // 开启全局中断
    P53 = 0; // 先关闭P53LED

    while (1)
        ;
}
