#include "PCF8563.h"
#include "GPIO.h"
#include "I2C.h"
#include "NVIC.h"
#include "Switch.h"
#include "Exti.h"

/****************  I2C GPIO初始化函数 *****************/
//SCL --> P32   SDA --> P33
static void GPIO_config() {
    GPIO_InitTypeDef gpio_init_struct;
    gpio_init_struct.Mode = GPIO_OUT_OD;//开漏输出
    gpio_init_struct.Pin = GPIO_Pin_2 | GPIO_Pin_3;//I2C总线所使用的两个IO
    GPIO_Inilize(GPIO_P3, &gpio_init_struct);
}

/****************  I2C初始化函数 *****************/
void I2C_config(void) {
    I2C_InitTypeDef I2C_InitStructure;

    I2C_InitStructure.I2C_Mode = I2C_Mode_Master;    //主从选择   I2C_Mode_Master, I2C_Mode_Slave
    I2C_InitStructure.I2C_Enable = ENABLE;            //I2C功能使能,   ENABLE, DISABLE
    I2C_InitStructure.I2C_MS_WDTA = DISABLE;            //主机使能自动发送,  ENABLE, DISABLE
    I2C_InitStructure.I2C_Speed = 13;                //总线速度=Fosc/2/(Speed*2+4),      0~63
    I2C_Init(&I2C_InitStructure);
    NVIC_I2C_Init(I2C_Mode_Master, DISABLE,
                  Priority_0);    //主从模式, I2C_Mode_Master, I2C_Mode_Slave; 中断使能, ENABLE/DISABLE; 优先级(低到高) Priority_0,Priority_1,Priority_2,Priority_3

    I2C_SW(I2C_P33_P32);                    //I2C_P14_P15,I2C_P24_P25,I2C_P33_P32
}


void PCF8563_init(void) {
    EAXSFR();//使能拓展寄存器
    GPIO_config();
    I2C_config();
}


void PCF8563_set(Clock_t c) {
    u8 p[NUMBER];
    u8 C = CENTURY;//世纪 0: 2000  1:2100
    //将十进制数据转成BCD格式数据-> 和取数据倒着来即可
    p[0] = ((c.second / 10) << 4) + (c.second % 10);
    p[1] = WRITE_BCD(c.minute);
    p[2] = WRITE_BCD(c.hour);
    p[3] = c.week;
    p[4] = WRITE_BCD(c.day);
    C = c.year < 2100 ? 0 : 1;
    p[5] = (C << 7) + WRITE_BCD(c.month);
    p[6] = WRITE_BCD(c.year % 100);//年的范围是 0 ~99，只取十位即可

    //数据写入
    I2C_WriteNbyte(PCF8563_DEV_ADDR, PCF8563_REG_TD, p, NUMBER);
}

void PCF8563_get(Clock_t *c) {
    u8 p[NUMBER];
    u8 C = CENTURY;//世纪 0: 2000  1:2100
    //通过I2C读取RTC时钟芯片的秒数据
    I2C_ReadNbyte(PCF8563_DEV_ADDR, PCF8563_REG_TD, p, NUMBER);

    //解析得到的数据
    c->second = ((p[0] >> 4) & 0x07) * 10 + (p[0] & 0x0F);//秒，高4位全取，低4位全取
    c->minute = ((p[1] >> 4) & 0x07) * 10 + (p[1] & 0x0F);//分，高4位全取，低4位全取
    c->hour = ((p[2] >> 4) & 0x03) * 10 + (p[2] & 0x0F);//时，高4位需要取两位，低4位全取
    c->day = ((p[3] >> 4) & 0x03) * 10 + (p[3] & 0x0F);//天，高4位需要取两位，低4位全取
    c->week = p[4] & 0x07;//周,只取低3位
    c->month = ((p[5] >> 4) & 0x01) * 10 + (p[5] & 0x0F);//月,高4位取1位，低4位全取
    C = p[5] >> 7;//世纪，只取最高位。 --> 0表示20xx年 1表示21xx年
    c->year = ((p[6] >> 4) & 0x0F) * 10 + (p[6] & 0x0F);//年，全都要;  --> 其范围是0~99

    /*根据BSD获取的年的范围是0~99，需要得到我们常见的"年"，需要将BSD得到的year与C所对应的值相加*/
    c->year += (C == 0 ? 2000 : 2100);
}

void PCF8563_set_alarm(Alarm_t a) {
    //存储闹钟设置值转成BCD码后的值
    u8 alarm[4];

    /*2.十进制转BCD码*/
    alarm[0] = WRITE_BCD(a.minute);
    alarm[0] = WRITE_BCD(a.hour);
    alarm[0] = WRITE_BCD(a.day);
    alarm[0] = WRITE_BCD(a.week_day);
    /*3.将分钟的地址作为写入地址，连续将分、时、天、周写入(会自动递增)*/
    I2C_WriteNbyte(PCF8563_DEV_ADDR, 0x09, alarm, 4);

}

void PCF8563_enable_alarm() {
    //存储状态控制寄存器2的值
    u8 Control_status_2 = 0;

    //先将状态控制寄存器2整个读出来
    I2C_ReadNbyte(PCF8563_DEV_ADDR, 0x01, &Control_status_2, 1);
    //清除AF闹钟标记，不清闹钟不会响
    Control_status_2 &= ~0x08;
    //将AIE位置1，使能闹钟
    Control_status_2 |= 0x02;
    //将修改后的状态寄存器2的值写回
    I2C_WriteNbyte(PCF8563_DEV_ADDR, 0x01, &Control_status_2, 1);
}


void PCF8536_alarm_clear_flag() {
//存储状态控制寄存器2的值
    u8 Control_status_2 = 0;

    //先将状态控制寄存器2整个读出来
    I2C_ReadNbyte(PCF8563_DEV_ADDR, 0x01, &Control_status_2, 1);
    //清除AF闹钟标记，不清闹钟不会响
    Control_status_2 &= ~0x08;
    //将修改后的状态寄存器2的值写回
    I2C_WriteNbyte(PCF8563_DEV_ADDR, 0x01, &Control_status_2, 1);
}
