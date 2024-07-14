#include "Config.h"
#include "GPIO.h"
#include "UART.h"
#include "EEPROM.h"
#include "Delay.h"
#include "NVIC.h"
#include "Switch.h"
#include <string.h>

/******************* IO���ú��� *******************/
void GPIO_config(void) {
    GPIO_InitTypeDef GPIO_InitStructure;        //�ṹ����

    GPIO_InitStructure.Pin = GPIO_Pin_0 | GPIO_Pin_1;        //ָ��Ҫ��ʼ����IO, GPIO_Pin_0 ~ GPIO_Pin_7
    GPIO_InitStructure.Mode = GPIO_PullUp;    //ָ��IO������������ʽ,GPIO_PullUp,GPIO_HighZ,GPIO_OUT_OD,GPIO_OUT_PP
    GPIO_Inilize(GPIO_P3, &GPIO_InitStructure);    //��ʼ��
}

//��ʼ��UART
void UART_config(void) {
    // >>> �ǵ����� NVIC.c, UART.c, UART_Isr.c <<<
    COMx_InitDefine COMx_InitStructure;                    //�ṹ����
    COMx_InitStructure.UART_Mode = UART_8bit_BRTx;    //ģʽ, UART_ShiftRight,UART_8bit_BRTx,UART_9bit,UART_9bit_BRTx
    COMx_InitStructure.UART_BRT_Use = BRT_Timer1;            //ѡ�����ʷ�����, BRT_Timer1, BRT_Timer2 (ע��: ����2�̶�ʹ��BRT_Timer2)
    COMx_InitStructure.UART_BaudRate = 115200ul;            //������, һ�� 110 ~ 115200
    COMx_InitStructure.UART_RxEnable = ENABLE;                //��������,   ENABLE��DISABLE
    COMx_InitStructure.BaudRateDouble = DISABLE;            //�����ʼӱ�, ENABLE��DISABLE
    UART_Configuration(UART1, &COMx_InitStructure);        //��ʼ������1 UART1,UART2,UART3,UART4

    NVIC_UART1_Init(ENABLE,
                    Priority_1);        //�ж�ʹ��, ENABLE/DISABLE; ���ȼ�(�͵���) Priority_0,Priority_1,Priority_2,Priority_3
    UART1_SW(UART1_SW_P30_P31);        // ����ѡ��, UART1_SW_P30_P31,UART1_SW_P36_P37,UART1_SW_P16_P17,UART1_SW_P43_P44
}

#define     Max_Length          100      //��дEEPROM���峤��
u8 xdata tmp[Max_Length];        //EEPROM��������

int main() {
    //Ҫ������������ʼ��ַ
    u16 addr_sector = 0x0040;
    //Ҫ�洢���ַ���
    char *str = "Doraemon";
    u16 str_len = strlen(str);

    EAXSFR();		/* ��չ�Ĵ�������ʹ�� */
    GPIO_config();
    UART_config();
    EA = 1;//ʧ���ж�


    //����������һ���Բ���һ������512�ֽڣ���0x0000��ʼ��0x01FF
    EEPROM_SectorErase(addr_sector);
    //д�����ݣ�String\int\long\float
    EEPROM_write_n(addr_sector, str, str_len);
    //��������ȡ����
    EEPROM_read_n(addr_sector, tmp, str_len);


    printf("д����ַ���:%s\n", str);
    //Ϊ�洢��ȡ�ַ��������������ַ�����������������ִ���
    tmp[str_len] = '\0';
    printf("�������ַ���Ϊ:%s\n", tmp);

    while (1) {
        printf("�������ַ���Ϊ:%s\n", tmp);
        delay_ms(250);
        delay_ms(250);
        delay_ms(250);
        delay_ms(250);
    }
}