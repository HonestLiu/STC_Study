/*需求: 控制P53引脚上连接的寄存器*/

// 定义端口模式配置寄存器
sfr P5M1 = 0xC9;
sfr P5M0 = 0xCA;
// 定义端口
sfr P5 = 0xc8;
// 定义P53引脚
sbit P53 = P5 ^ 3;

// 延时函数
void Delay1000us(void) //@11.0592MHz
{
    unsigned char data i, j;

    i = 15;
    j = 90;
    do
    {
        while (--j)
            ;
    } while (--i);
}

int main()
{

    // 设置P5端口的3号引脚为双向输出，置0，用`&= ~`运算即可
    P5M0 &= ~0x80;
    P5M1 &= ~0x80;

    while (1)
    {
        P53 = 1; // 设置P53引脚为高电平，此时该引脚上的LED亮起
        Delay1000us();
        P53 = 0; // 设置P53引脚为低电平
        Delay1000us();
    } // 卡住，避免程序结束用的
}