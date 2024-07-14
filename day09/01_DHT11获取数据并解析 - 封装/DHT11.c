#include "DHT11.h"
#include "GPIO.h"
#include "Delay.h"

// level 当前电平状态
#define wait_level_change(level, min, max, description)                  \
    cnt = 0;                                                                \
    do {                                                                    \
        delay_1us();                                                        \
        cnt++;                                                              \
    } while(DHT11 == level);                                                  \
    if (cnt < min || cnt > max) {                                           \
        printf("err: [%s] [%d]us 不满足!\n", description, cnt);              \
        return -1;                                                          \
    }

//延时1us的函数
void delay_1us() {
    NOP10();
}

// 初始化GPIO-P46为准双向
static void GPIO_config() {
    GPIO_InitTypeDef gpio_init_struct;
    gpio_init_struct.Mode = GPIO_PullUp;
    gpio_init_struct.Pin = GPIO_Pin_6;
    GPIO_Inilize(GPIO_P4, &gpio_init_struct);
}

void DHT11_init() {
    GPIO_config();
}

char on_read(u8 *dat) {
    u16 cnt = 0;
    char i = 0, j = 0;//用来遍历的变量
    /*1.主机起始信号拉低时间: 主机起始拉低DHT11引脚的电平*/
    DHT11 = 0;
    delay_ms(20);//保持低电平在一个典型值时长->20ms
    /*2.主机释放总线时间: 等待由高电平转换为低电平 时间范围[10,50](50是取了下一个状态的中间值，从而判断通信失败)*/
    DHT11 = 1;
    //计数统计，每1us计数一次，统计消耗的时间
    cnt = 0;
    do {
        delay_1us();
        cnt++;
    } while (DHT11 == 1 && cnt < 50);
    //根据统计消耗时间判断此阶段是否成功
    if (cnt < 10 || cnt > 35) {
        printf("主机释放时间超时\n");
        return -1;
    }
    /*3.响应低电平时间: 低电平时间 [78:88]*/
    wait_level_change(0, 78, 88, "响应低电平时间超时");
    /*4.响应高电平时间: 高电平时间 [80:92]*/
    wait_level_change(1, 80, 92, "响应高电平时间超时");
    /*5.解析接收到的电平，根据高低电平时长区分0或1*/
    for (i = 0; i < 5; i++) {//需要读5个byte,每读8个bit循环一次
        for (j = 7; j >= 0; j--) {//每次循环读8个bit(数据写入必须从右往左写入⚠)
            u8 bit_dat;
            //统计0、1的数据低电平时间
            wait_level_change(0, 48, 58, "data低电平时间");
            //统计高电平时间，用以判断是0还是1
            wait_level_change(1, 23, 74, "data高电平时间");
            //判断高低电平，然后写入到存储数组中(取中间值47,小于47的为0,高于47的为1)
            bit_dat = (cnt >= 48 ? 1 : 0);
            dat[i] |= (bit_dat << j);//位移写入该bit在该byte的对应位置
        }
    }

    //校验数据；校验位(第4个数据)＝湿度高位+湿度低位+温度高位+温度低位
    if ((dat[0] + dat[1] + dat[2] + dat[3]) != dat[4]) {
        printf("Data validation failure!!");//数据校验失败
        return -1;
    }
    //成功返回0，实际对数据的处理在main函数中进行
    return 0;
}


char DHT11_get_info(float *humidity, float *temperature) {
    char ret = -1;
    u8 dat[5] = {0};    // 准备5个字节的容器,接收结果

    ret = on_read(dat);
    if (ret < 0) {    //数据校验失败
        return -1;
    }
    /*1.计算湿度*/
    *humidity = (int) dat[0];
    /*2.计算温度*/
    // 温度值: 温度高位的后7bit + 温度低位 * 0.1
    *temperature = dat[2] + ((dat[3] & 0x7F) * 0.1);
    // 温度的正负: 取温度高位的最高位
    if ((dat[3] & 0x80) > 0) {//为1则为负数
        //对获取的温度值进行取反、
        *temperature = -*temperature;
    }

    return 0;

}
