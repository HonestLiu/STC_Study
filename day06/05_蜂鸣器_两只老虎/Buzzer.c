#include "Buzzer.h"
#include "STC8H_PWM.h"
#include "Switch.h"
#include "GPIO.h"
#include "NVIC.h"


// 初始化蜂鸣器GPIO
static void gpio_config(void) {
    // 初始化蜂鸣器兼PWM引脚 - P00
    GPIO_InitTypeDef gpio_init_struct;
    gpio_init_struct.Pin = BUZZER_PIN;
    gpio_init_struct.Mode = GPIO_OUT_PP;//推挽输出
    GPIO_Inilize(BUZZER_GPIO, &gpio_init_struct);
}

// 初始化PWM5-对应P00
void PWM_config(u16 hz_value) {
    PWMx_InitDefine PWMx_InitStructure;

    //通过八度音阶计算PWM的周期，这个值根据传入的音阶的不同也会改变⭐
    u16 period = MAIN_Fosc / hz_value;

    // 配置PWM5
    PWMx_InitStructure.PWM_Mode = CCMRn_PWM_MODE1; // 模式
    PWMx_InitStructure.PWM_Duty = (u16) (period * 0.5); // PWM占空比时间,这里控制蜂鸣器，故固定为周期的一半即可⭐
    PWMx_InitStructure.PWM_EnoSelect = ENO5P; // 输出通道选择
    PWM_Configuration(PWM5, &PWMx_InitStructure); // 初始化PWM

    // 配置PWMB
    PWMx_InitStructure.PWM_Period = period - 1;    // 周期时间, 设置为根据八度音阶计算的周期即可⭐
    PWMx_InitStructure.PWM_DeadTime = 0;           // 死区发生器设置, 0~255
    PWMx_InitStructure.PWM_MainOutEnable = ENABLE; // 主输出使能, ENABLE,DISABLE
    PWMx_InitStructure.PWM_CEN_Enable = ENABLE; // 使能计数器, ENABLE,DISABLE
    PWM_Configuration(PWMB, &PWMx_InitStructure); // 初始化PWM通用寄存器,  PWMA,PWMB

    // 切换PWM通道
    PWM5_SW(PWM5_SW_P00); // PWM5_SW_P20,PWM5_SW_P17,PWM5_SW_P00,PWM5_SW_P74

    // 初始化PWMB的中断
    NVIC_PWM_Init(PWMB, DISABLE, Priority_0);
}

void Buzzer_init() {
    EAXSFR(); /* 扩展寄存器访问使能 */
    gpio_config();
}

void Buzzer_play(u16 hz_val) {
    //不断传入八度音阶，修改PWM的周期，从而使用蜂鸣器输出不同音节的声音
    PWM_config(hz_val);
}

//依照传入的音播放声音-对Buzzer_play的进一步封装
/*八度音阶数组*/
//			 C`	   D`     E`   F`	  G`	A`	  B` C``
u16 hz[] = {1047, 1175, 1319, 1397, 1568, 1760, 1976, 2093};

void Buzzer_beep(u8 idx) {
    //为了更符合人的习惯，用户传入音阶是从1开始，所以这里对传入值进行减一操作
    u16 hz_val = hz[idx - 1];
    //调用Buzzer_play播放声音
    Buzzer_play(hz_val);
}

/*停止蜂鸣器-失能主输出和计数器*/
void Buzzer_stop() {
    PWMx_InitDefine PWMx_InitStructure;
    PWMx_InitStructure.PWM_MainOutEnable = DISABLE; // 主输出失能
    PWMx_InitStructure.PWM_CEN_Enable = DISABLE; // 计数器失能
    PWM_Configuration(PWMB, &PWMx_InitStructure); // 修改PWM通用寄存器参数
}

