#ifndef __DHT11_H
#define __DHT11_H

#include "Config.h"

//DHT11的数据通信引脚
#define DHT11 P46
/**
 * @description DHT11初始化函数
 * @note 如需修改DHT11的引脚，可修改DHT11.h的宏
 * */
void DHT11_init();

/**
 * @description 获取DHT11数据
 * @param humidity 湿度
 * @param temperature 温度值
 * @return 0 数据获取成功
 * @return -1 数据获取失败
 * */
char DHT11_get_info(float *humidity, float *temperature);

#endif
