#ifndef __PCF8563_H__
#define __PCF8563_H__

#include "Config.h"

//设备地址
#define PCF8563_DEV_ADDR (0x51 << 1)
// 存储地址: 时间(秒)存储地址
#define PCF8563_REG_TD 0x02
//要读的位数,其会自动进位，即读完秒，会自动移动到秒和时的地方去获取
#define NUMBER 7
//世纪设置,0: 2000  1:2100
#define CENTURY 0
//参数结构体-u8 second, minute, hour, day, week, month
typedef struct {
    u16 year;
    u8 month;
    u8 week;
    u8 day;
    u8 hour;
    u8 minute;
    u8 second;
} Clock_t;

//初始化函数
void PCF8563_init(void);

//设置时钟信息函数
void PCF8563_set(Clock_t *c);

//获取时钟信息函数->通过参数指针返回要取的数据
void PCF8563_get(Clock_t *c);


#endif
