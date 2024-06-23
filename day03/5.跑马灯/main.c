#include "Config.h"
#include "GPIO.h"
#include "Delay.h"

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

/* 初始化GPIO */
void gpio_config()
{
    GPIO_InitTypeDef gpio_config_struct = {0};
    // 初始化SW引脚-> 电源总开关
    gpio_config_struct.Mode = GPIO_OUT_PP; // 需要高电平控制三极管，故使用推挽输出
    gpio_config_struct.Pin = GPIO_Pin_5;
    GPIO_Inilize(GPIO_P4, &gpio_config_struct);

    // 初始化LED
    gpio_config_struct.Mode = GPIO_PullUp;
    gpio_config_struct.Pin = GPIO_Pin_4 | GPIO_Pin_5;
    GPIO_Inilize(GPIO_P1, &gpio_config_struct);

    gpio_config_struct.Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_6 | GPIO_Pin_7;
    GPIO_Inilize(GPIO_P2, &gpio_config_struct);
}

void main()
{
    char i;
    gpio_config();

    /**
     * @note Sw:LED灯组总开关，连接一个PNP三极管，作无接触开关，输出高电平时闭合，输出低电平时导通
     **/
    LED_SW = LOW; // 打开总开关

    /**
     * @note LED接口，端口另一侧为SW控制的VCC，端口处输出低电平即可点亮LED
     */

    // 初始化所有LED灯->点亮0 熄灭 1
    LED1 = 0;
    LED2 = 0;
    LED3 = 0;
    LED4 = 0;
    LED5 = 0;
    LED6 = 0;
    LED7 = 0;
    LED8 = 0;

    while (1)
    {
        /*for (i = 8; i < 0; i--)
        {
            i == 0 ? (LED1 = 0) : (LED1 = 1);

        }*/
        /* 利用for循环，依次点亮LED后依次关闭LED */
        for (i = 0; i < 8; i++)
        {
            LED1 = (i == 0 ? 0 : 1);
            LED2 = (i == 1 ? 0 : 1);
            LED3 = (i == 2 ? 0 : 1);
            LED4 = (i == 3 ? 0 : 1);
            LED5 = (i == 4 ? 0 : 1);
            LED6 = (i == 5 ? 0 : 1);
            LED7 = (i == 6 ? 0 : 1);
            LED8 = (i == 7 ? 0 : 1);
            delay_ms(100);
        }

        for (i = 7; i >= 0; i--)
        {
            LED1 = (i == 0 ? 0 : 1);
            LED2 = (i == 1 ? 0 : 1);
            LED3 = (i == 2 ? 0 : 1);
            LED4 = (i == 3 ? 0 : 1);
            LED5 = (i == 4 ? 0 : 1);
            LED6 = (i == 5 ? 0 : 1);
            LED7 = (i == 6 ? 0 : 1);
            LED8 = (i == 7 ? 0 : 1);
            delay_ms(100);
        }
    }
}
