#include "Motors.h"
#include "../Lib/GPIO.h"
#include "../Lib/NVIC.h"
#include "../Lib/STC8H_PWM.h"
#include "../Lib/Switch.h"

#define LQ_B P17
#define LQ_F P16
#define RQ_B P15
#define RQ_F P14
#define LH_B P20
#define LH_F P21
#define RH_F P22
#define RH_B P23

// speed范围: -100 ~ 100
// duty范围: 0 ~ 100
int Speed2Duty(int speed) {
  // speed /2 => -50 ~ 50
  // speed /2 + 50 => 0 ~ 100
  return speed /2 + 50;
}
    
#define PERIOD (MAIN_Fosc / 1000)
PWMx_Duty dutyA;
void Motor_config(MotorCfg *mc) { // 通过传入的duty值调整占空比
  PWMx_InitDefine PWMx_InitStructure;

  int LH_duty = Speed2Duty(mc->LH_speed);
  int RH_duty = Speed2Duty(mc->RH_speed);
  int RQ_duty = Speed2Duty(mc->RQ_speed);
  int LQ_duty = Speed2Duty(mc->LQ_speed);

  // 配置PWM1 LH
  PWMx_InitStructure.PWM_Mode = CCMRn_PWM_MODE1; // 模式
  PWMx_InitStructure.PWM_Duty = PERIOD * LH_duty / 100; // PWM占空比时间
  PWMx_InitStructure.PWM_EnoSelect = (mc->LH_speed == 0 ? 0:(ENO1P | ENO1N)); // 输出通道选择(当速度为0时不选择)
  PWM_Configuration(PWM1, &PWMx_InitStructure); // 初始化PWM
  // 配置PWM2 RH
  PWMx_InitStructure.PWM_Mode = CCMRn_PWM_MODE2;  
  PWMx_InitStructure.PWM_Duty = PERIOD * RH_duty / 100; // PWM占空比时间
  PWMx_InitStructure.PWM_EnoSelect =  (mc->RH_speed == 0 ? 0 : (ENO2P | ENO2N)); // 输出通道选择(当速度为0时不选择)
  PWM_Configuration(PWM2, &PWMx_InitStructure); // 初始化PWM
  // 配置PWM3 RQ
  PWMx_InitStructure.PWM_Mode = CCMRn_PWM_MODE1; // 模式
  PWMx_InitStructure.PWM_Duty = PERIOD * RQ_duty / 100; // PWM占空比时间, 0~Period
  PWMx_InitStructure.PWM_EnoSelect = (mc->RQ_speed == 0 ? 0 : (ENO3P | ENO3N)); // 输出通道选择(当速度为0时不选择)
  PWM_Configuration(PWM3, &PWMx_InitStructure);
  // 配置PWM4 LQ
  PWMx_InitStructure.PWM_Mode = CCMRn_PWM_MODE1; // 模式,
  PWMx_InitStructure.PWM_Duty = PERIOD * LQ_duty / 100; // PWM占空比时间, 0~Period
  PWMx_InitStructure.PWM_EnoSelect = (mc->LQ_speed == 0 ? 0 : (ENO4P | ENO4N)); //输出通道选择(当速度为0时不选择)
  PWM_Configuration(PWM4, &PWMx_InitStructure);

  // 配置PWMA
  PWMx_InitStructure.PWM_Period = PERIOD - 1;    // 周期时间,   0~65535
  PWMx_InitStructure.PWM_DeadTime = 0;           // 死区发生器设置, 0~255
  PWMx_InitStructure.PWM_MainOutEnable = ENABLE; // 主输出使能, ENABLE,DISABLE
  PWMx_InitStructure.PWM_CEN_Enable = ENABLE; // 使能计数器, ENABLE,DISABLE
  PWM_Configuration(PWMA,  &PWMx_InitStructure); // 初始化PWM通用寄存器,  

  // 切换PWM通道
  // LH
  PWM1_SW(PWM1_SW_P20_P21); // PWM1_SW_P10_P11,PWM1_SW_P20_P21,PWM1_SW_P60_P61
  // RH
  PWM2_SW(PWM2_SW_P22_P23); // PWM2_SW_P12_P13,PWM2_SW_P22_P23,PWM2_SW_P62_P63
  // RQ
  PWM3_SW(PWM3_SW_P14_P15); // PWM3_SW_P14_P15,PWM3_SW_P24_P25,PWM3_SW_P64_P65
  // LQ
  PWM4_SW(PWM4_SW_P16_P17); // PWM4_SW_P16_P17,PWM4_SW_P26_P27,PWM4_SW_P66_P67,PWM4_SW_P34_P33

  // 初始化PWMA的中断
  NVIC_PWM_Init(PWMA, DISABLE, Priority_0);
}

void GPIO_Config(void) {
  // 初始化为准双向即可
  P1_MODE_IO_PU(GPIO_Pin_7 | GPIO_Pin_6 | GPIO_Pin_4 | GPIO_Pin_5);
  P2_MODE_IO_PU(GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3);
}

void Motor_init() {
  // 只初始化GPIO，PWM每次调整设置一次(某些芯片直接修改可能会出现问题)
  GPIO_Config();
}

void Motor_forward(int speed) {
  MotorCfg mc;
  mc.LQ_speed = speed;
  mc.RQ_speed = speed;
  mc.LH_speed = speed;
  mc.RH_speed = speed;
  Motor_config(&mc);
  }
  void Motor_backward(int speed) {
  MotorCfg mc;
  mc.LQ_speed = -speed;
  mc.RQ_speed = -speed;
  mc.LH_speed = -speed;
  mc.RH_speed = -speed;
  Motor_config(&mc);
}

void Motor_left(int speed) {
   MotorCfg mc;
  mc.LQ_speed = 0;
  mc.RQ_speed = speed;
  mc.LH_speed = 0;
  mc.RH_speed = speed;
  Motor_config(&mc);
}
void Motor_right(int speed) {
   MotorCfg mc;
  mc.LQ_speed = speed;
  mc.RQ_speed = 0;
  mc.LH_speed = speed;
  mc.RH_speed = 0;
  Motor_config(&mc);
}

void Motor_stop() {
   MotorCfg mc;
  mc.LQ_speed = 0;
  mc.RQ_speed = 0;
  mc.LH_speed = 0;
  mc.RH_speed = 0;
  Motor_config(&mc);
}