#ifndef __PCF8563_H__
#define __PCF8563_H__

#include "Config.h"

/*#define PCF8563_TIMER_ENABLE
#define PCF8563_ALARM_ENABLE*/

//设备地址
#define PCF8563_DEV_ADDR 0xA2
// 存储地址: 时间(秒)存储地址
#define PCF8563_REG_TD 0x02
//要读的位数,其会自动进位，即读完秒，会自动移动到秒和时的地方去获取
#define NUMBER 7
//世纪设置,0: 2000  1:2100
#define CENTURY 0
// 十位取出左移4位 + 个位 (得到BCD数)
#define WRITE_BCD(val)    ((val / 10) << 4) + (val % 10)

//日期设置结构体，参数结构体-u8 second, minute, hour, day, week, month
typedef struct {
    u16 year;
    u8 month;
    u8 week;
    u8 day;
    u8 hour;
    u8 minute;
    u8 second;
} Clock_t;

//闹钟设置结构体
typedef struct {
    u8 week_day;
    u8 day;
    u8 hour;
    u8 minute;
} Alarm_t;

//定时器时钟源枚举,设置上时钟源时，直接将下面的十进制丢到定时器控制寄存器第一位即可
typedef enum {
    Hz4096 = 0,
    Hz64 = 1,
    Hz1 = 2,
    Hz60_1 = 3,
} Time_Freq;

/***************************初始化函数*************************************/

//初始化函数
void PCF8563_init(void);

//设置时钟信息函数
void PCF8563_set(Clock_t c);

//获取时钟信息函数->通过参数指针返回要取的数据
void PCF8563_get(Clock_t *c);

/* *************************闹钟*******************************************/
//闹钟设置结构体
void PCF8563_set_alarm(Alarm_t a);

//使能闹钟
void PCF8563_enable_alarm();

//失能闹钟
void PCF8563_disable_alarm();

//清除闹钟标记
void PCF8536_alarm_clear_flag();

/****************************定时器******************************************/
//闹钟设置结构体
void PCF8563_set_timer(Time_Freq freq, u8 countDown);

//使能闹钟
void PCF8563_enable_timer();

//失能闹钟
void PCF8563_disable_timer();

//清除闹钟标记
void PCF8536_timer_clear_flag();

/***************************中断触发函数(main实现)****************************/

//闹钟中断触发调用函数
#ifdef PCF8563_ALARM_ENABLE
extern void PCF8536_on_alarm();
#endif

//定时器中断触发调用函数
#ifdef PCF8563_TIMER_ENABLE
extern void PCF8536_on_timer();
#endif
#endif
