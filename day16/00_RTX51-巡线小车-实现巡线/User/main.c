// �ⲿ�豸
#include "../Drive/Buzzer.h"
#include "../Drive/Key.h"
#include "../Drive/Light.h"
#include "../Drive/Motors.h"
#include "../Drive/Track.h"
#include "../Drive/Ultrasonic.h"
#include "../Drive/battery.h"

// Ƭ������
#include "../Lib/Config.h"
#include "../Lib/NVIC.h"
#include "../Lib/Switch.h"
#include "../Lib/UART.h"

#include "RTX51TNY.h" //RTX51��ϵͳͷ�ļ�
#include <cstdio>
#include <stdio.h>

void UART_config(void) {
  // >>> �ǵ���� NVIC.c, UART.c, UART_Isr.c <<<
  COMx_InitDefine COMx_InitStructure; // �ṹ����
  COMx_InitStructure.UART_Mode =
      UART_8bit_BRTx; // ģʽ,
                      // UART_ShiftRight,UART_8bit_BRTx,UART_9bit,UART_9bit_BRTx
  COMx_InitStructure.UART_BRT_Use =
      BRT_Timer1; // ѡ�����ʷ�����, BRT_Timer1, BRT_Timer2 (ע��:
                  // ����2�̶�ʹ��BRT_Timer2)
  COMx_InitStructure.UART_BaudRate = 115200ul; // ������, һ�� 110 ~ 115200
  COMx_InitStructure.UART_RxEnable = ENABLE; // ��������,   ENABLE��DISABLE
  COMx_InitStructure.BaudRateDouble = DISABLE; // �����ʼӱ�, ENABLE��DISABLE
  UART_Configuration(
      UART1, &COMx_InitStructure); // ��ʼ������1 UART1,UART2,UART3,UART4

  NVIC_UART1_Init(ENABLE,
                  Priority_1); // �ж�ʹ��, ENABLE/DISABLE; ���ȼ�(�͵���)
                               // Priority_0,Priority_1,Priority_2,Priority_3
  UART1_SW(
      UART1_SW_P30_P31); // ����ѡ��,
                         // UART1_SW_P30_P31,UART1_SW_P36_P37,UART1_SW_P16_P17,UART1_SW_P43_P44
}

void sys_init() {
  EAXSFR();
  // Ƭ�������ʼ��
  UART_config(); // ��ʼ��UART

  // ������ʼ��
  Light_init();      // ��ʼ��ת���
  KEY_init();        // ��ʼ����ť
  Battery_init();    // ��ʼ��ADC��Դ���
  buzzer_init();     // ��ʼ��������
  Ultrasonic_init(); // ��ʼ��������ģ��
  Track_init();      // ��ʼ��Ѳ��ģ��
  Motor_init();      // ��ʼ���������

  EA = 1;
}

void start_main() _task_ 0 {
  sys_init();
  os_create_task(1); // ������������

  // �����Լ�
  os_delete_task(0);
}

u8 track_state = 0; // Ѳ�߹���״̬ 0��δ���� 1:������
void KEY_down() {
  Light_on(ALL);
  if (!track_state) {
    printf("����Ѳ��\n");
    os_create_task(2); // ����Ѳ������
  } else {
    printf("�ر�Ѳ��\n");
    os_delete_task(2); // ����Ѳ������
  }
  // ����Ѳ��״̬
  track_state = !track_state;
}

void KEY_up() { Light_off(ALL); }

void task_Key() _task_ 1 {

  while (1) {
    KEY_scan();
    os_wait2(K_TMO, 2);
  }
}

void task_Track() _task_ 2 {
	int pos = 0, last_pos = -10;
  while (1) {
    

    pos = Track_get_position();
    //���pos����last_pos����������ǰ���ã���Լϵͳ��Դ
    if (pos == last_pos) {
      os_wait2(K_TMO, 2);//����һС�ᣬ�������ظ�����Track_get_position
      continue;
    }
    //����last_pos
    last_pos = pos;
		printf("pos: %d\n", pos);
		
    if (pos < 0) {
      printf("Motor_left\n");
      Motor_left(60);
    } else if (pos > 0) {
      printf("Motor_right\n");
      Motor_right(60);
    } else {
      printf("Motor_forward\n");
      Motor_forward(70);
    }

    os_wait2(K_TMO, 4); // 5 * 4 = 20ms,ÿ20ms��ȡһ��
  }
}