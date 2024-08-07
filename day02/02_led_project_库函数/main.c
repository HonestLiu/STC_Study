#include "Config.h"
#include "GPIO.h"
#include "Delay.h"

void gpioConfig()
{
    // 配置IO模式为推挽模式
    GPIO_InitTypeDef gpio_config;
    gpio_config.Mode = GPIO_OUT_PP; // 推挽模式
    gpio_config.Pin = GPIO_P5;//引脚5
    GPIO_Inilize(GPIO_Pin_3, &gpio_config);
}

int main(void)
{
    /* 使用宏函数
    //配置IO模式为准双向模式，具体的宏去头文件中找就好，P5:端口 GPIO_Pin_3:是引脚
    P5_MODE_IO_PU(GPIO_Pin_3);
    */

    /*使用初始化函数*/
    gpioConfig();

    while (1)
    {
        P53 = 1; // 点灯
        delay_ms(250);
        delay_ms(250);
        delay_ms(250);
        delay_ms(250);
        P53 = 0; // 关灯
        delay_ms(250);
        delay_ms(250);
        delay_ms(250);
        delay_ms(250);
    }
}