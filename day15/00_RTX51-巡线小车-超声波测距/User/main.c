#include "../Drive/Key.h"
#include "../Drive/Light.h"
#include "../Drive/battery.h"
#include "../Drive/Buzzer.h"
#include "../Drive/Ultrasonic.h"
#include "../Lib/Config.h"
#include "../Lib/UART.h"
#include "../Lib/NVIC.h"
#include "../Lib/Switch.h"
#include "RTX51TNY.h" //RTX51��ϵͳͷ�ļ�


void UART_config(void) {
	// >>> �ǵ���� NVIC.c, UART.c, UART_Isr.c <<<
    COMx_InitDefine		COMx_InitStructure;					//�ṹ����
    COMx_InitStructure.UART_Mode      = UART_8bit_BRTx;	//ģʽ, UART_ShiftRight,UART_8bit_BRTx,UART_9bit,UART_9bit_BRTx
    COMx_InitStructure.UART_BRT_Use   = BRT_Timer1;			//ѡ�����ʷ�����, BRT_Timer1, BRT_Timer2 (ע��: ����2�̶�ʹ��BRT_Timer2)
    COMx_InitStructure.UART_BaudRate  = 115200ul;			//������, һ�� 110 ~ 115200
    COMx_InitStructure.UART_RxEnable  = ENABLE;				//��������,   ENABLE��DISABLE
    COMx_InitStructure.BaudRateDouble = DISABLE;			//�����ʼӱ�, ENABLE��DISABLE
    UART_Configuration(UART1, &COMx_InitStructure);		//��ʼ������1 UART1,UART2,UART3,UART4

  	NVIC_UART1_Init(ENABLE,Priority_1);		//�ж�ʹ��, ENABLE/DISABLE; ���ȼ�(�͵���) Priority_0,Priority_1,Priority_2,Priority_3
    UART1_SW(UART1_SW_P30_P31);		// ����ѡ��, UART1_SW_P30_P31,UART1_SW_P36_P37,UART1_SW_P16_P17,UART1_SW_P43_P44
}

void sys_init() {
  EAXSFR();

  UART_config();//��ʼ��UART
  Light_init();//��ʼ��ת���
  KEY_init();//��ʼ����ť
  Battery_init(); // ��ʼ��ADC��Դ���
  buzzer_init();  // ��ʼ��������
  Ultrasonic_init();//��ʼ��������ģ��

  EA = 1;
}

void start() _task_ 0 {
  sys_init();
  os_create_task(1); // ��������1
                     // os_create_task(2);//��������2

  // �����Լ�
  os_delete_task(0);
}

void KEY_down() {
  char dis_res = 0;
  float voltage;//ADC���������ֵ
  float distance; // ��������෵�ؽ��

  Light_on(ALL);

  Battery_get_voltage(&voltage); // ��ȡ��ԴADC�����

  buzzer_beep(); // ��Դ��������������

  dis_res = Ultrasonic_get_distance(&distance);
  if (dis_res == 0)
  {
    printf("�����ȡ�ɹ�: %.2f cm\n", distance);
  }else if (dis_res == 1)
  {
    printf("С�ڿɲ����: %.2f cm\n",distance);
  }else if (dis_res == 2)
  {
    printf("���ڿɲ����: %.2f cm\n",distance);
  }else
  {
    printf("�����ȡʧ��");
  }
  
  }

void KEY_up() { Light_off(ALL); }

void task1() _task_ 1 {

  while (1) {
    KEY_scan();
    os_wait2(K_TMO, 2);
  }
}

  /* void task2() _task_ 2 {

    while (1) {

    }
  } */