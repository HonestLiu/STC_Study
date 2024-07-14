#ifndef __MOTORS_H__
#define __MOTORS_H__

#include "../Lib/Config.h"

// 速度配置结构体
// 范围: -100 ~ +100
typedef struct {
  int LQ_speed;
  int LH_speed;
  int RQ_speed;
  int RH_speed;
} MotorCfg;

void Motor_init();              // 初始化函数
void Motor_forward(int speed);  // 前进函数
void Motor_backward(int speed); // 后退函数
void Motor_left(int speed);     // 左转函数
void Motor_right(int speed);    // 右转函数

void Motor_stop(); // 停止函数

#endif // !__MOTORS_H__
