#include "Config.h"
#include "GPIO.h"
#include "UART.h"
#include "Delay.h"
#include "Switch.h"
#include "NVIC.h"

#define Row1 P34
#define Row2 P35
#define Row3 P40
#define Row4 P41
#define COL1 P03
#define COL2 P06
#define COL3 P07
#define COL4 P17


//将矩阵键盘的相关引脚全部初始化为准双向
void GPIO_config() {
    P0_MODE_IO_PU(GPIO_Pin_3 | GPIO_Pin_6 | GPIO_Pin_7);
    P1_MODE_IO_PU(GPIO_Pin_7);
    P3_MODE_IO_PU(GPIO_Pin_4 | GPIO_Pin_5);
    P4_MODE_IO_PU(GPIO_Pin_0 | GPIO_Pin_1);
}

void UART_config(void) {
    // >>> 记得添加 NVIC.c, UART.c, UART_Isr.c <<<
    COMx_InitDefine COMx_InitStructure;                    //结构定义
    COMx_InitStructure.UART_Mode = UART_8bit_BRTx;    //模式, UART_ShiftRight,UART_8bit_BRTx,UART_9bit,UART_9bit_BRTx
    COMx_InitStructure.UART_BRT_Use = BRT_Timer1;            //选择波特率发生器, BRT_Timer1, BRT_Timer2 (注意: 串口2固定使用BRT_Timer2)
    COMx_InitStructure.UART_BaudRate = 115200ul;            //波特率, 一般 110 ~ 115200
    COMx_InitStructure.UART_RxEnable = ENABLE;                //接收允许,   ENABLE或DISABLE
    COMx_InitStructure.BaudRateDouble = DISABLE;            //波特率加倍, ENABLE或DISABLE
    UART_Configuration(UART1, &COMx_InitStructure);        //初始化串口1 UART1,UART2,UART3,UART4

    NVIC_UART1_Init(ENABLE,
                    Priority_1);        //中断使能, ENABLE/DISABLE; 优先级(低到高) Priority_0,Priority_1,Priority_2,Priority_3
    UART1_SW(UART1_SW_P30_P31);        // 引脚选择, UART1_SW_P30_P31,UART1_SW_P36_P37,UART1_SW_P16_P17,UART1_SW_P43_P44
}

//按下为低电平，抬起为高电平
#define DOWN 0
#define UP 1

//记录按钮前一刻的状态
/*u8 key_last_state[16] = {
        UP, UP, UP, UP,
        UP, UP, UP, UP,
        UP, UP, UP, UP,
        UP, UP, UP, UP
};*/
u16 key_last_state = 0xFFFF;

//按键之前的状态是否为抬起
#define IS_KEY_UP(row_id, col_id) (key_last_state & (1 << (4 * row_id + col_id))) > 0
//按键之前的状态是否为按下
#define IS_KEY_DOWN(row_id, col_id) (key_last_state & (1 << (4 * row_id + col_id))) == 0
//修改按键当前状态为按下
#define SET_KEY_DOWN(row_id, col_id) key_last_state &=~ (1 << (4 * row_id + col_id))
//修改按键当前状态为抬起
#define SET_KEY_UP(row_id, col_id) key_last_state |= (1 << (4 * row_id + col_id))


//根据传入的行id改变行的电平状态，以进入监听状态
void chang_row_state(u8 row_id) {
    Row1 = (row_id == 0 ? 0 : 1);
    Row2 = (row_id == 1 ? 0 : 1);
    Row3 = (row_id == 2 ? 0 : 1);
    Row4 = (row_id == 3 ? 0 : 1);
}

//根据列id获取列的电平状态
u8 col_id_get_state(u8 col_id) {
    switch (col_id) {
        case 0:
            return COL1;
            break;
        case 1:
            return COL2;
            break;
        case 2:
            return COL3;
            break;
        case 3:
            return COL4;
            break;
        default:
            return 0;
            break;
    }
}

void key_scan() {
    u8 row_id;//行号ID
    u8 col_id;//列号ID

    //循环行
    for (row_id = 0; row_id < 4; row_id++) {
        //循环到哪一行，哪一行就切换成低电平(进入监听)
        chang_row_state(row_id);
        //遍历每一列的电平状态，以判断是否按下
        for (col_id = 0; col_id < 4; col_id++) {
            //按键按下
            if (col_id_get_state(col_id) == DOWN && IS_KEY_UP(row_id, col_id)) {
                printf("ROW:%d COL：%d of Key Down\n", (int) row_id, (int) col_id);
                //更新状态
                SET_KEY_DOWN(row_id, col_id);
            } else if (col_id_get_state(col_id) == UP && IS_KEY_DOWN(row_id, col_id)) {
                printf("ROW:%d COL：%d of Key Up\n", (int) row_id, (int) col_id);
                SET_KEY_UP(row_id, col_id);
            }
        }

    }
}

int main() {
    GPIO_config();
    UART_config();

    EA = 1;
    while (1) {
        key_scan();
        //每25ms监听一次按键的变化
        delay_ms(25);
    }
}