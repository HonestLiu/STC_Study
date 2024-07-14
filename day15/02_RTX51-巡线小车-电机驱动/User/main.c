// �ⲿ�豸
#include "../Drive/Buzzer.h"
#include "../Drive/Key.h"
#include "../Drive/Light.h"
#include "../Drive/Track.h"
#include "../Drive/Ultrasonic.h"
#include "../Drive/battery.h"
// Ƭ������
#include "../Lib/Config.h"
#include "../Lib/GPIO.h"
#include "../Lib/NVIC.h"
#include "../Lib/Switch.h"
#include "../Lib/UART.h"

#include "RTX51TNY.h" //RTX51��ϵͳͷ�ļ�

#define RQ_B P15
#define RQ_F P14

void GPIO_Config(void) {
  // ��ʼ��Ϊ׼˫�򼴿�
  P1_MODE_IO_PU(GPIO_Pin_4 | GPIO_Pin_5);
}

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
  GPIO_Config();

  // ������ʼ��
  Light_init();      // ��ʼ��ת���
  KEY_init();        // ��ʼ����ť
  Battery_init();    // ��ʼ��ADC��Դ���
  buzzer_init();     // ��ʼ��������
  Ultrasonic_init(); // ��ʼ��������ģ��
  Track_init();      // ��ʼ��Ѳ��ģ��

  EA = 1;
}

void start_main() _task_ 0 {
  sys_init();
  os_create_task(1); // ������������
  // os_create_task(2); // ����Ѳ������

  // �����Լ�
  os_delete_task(0);
}

void KEY_down() {
  static u8 last_machinery_state = 1;

  Light_on(ALL);
  if (last_machinery_state) // ǰ��
  {
    RQ_B = 1;
    RQ_F = 0;
  } else { // ����
    RQ_B = 0;
    RQ_F = 1;
  }
  // ȡ�����״̬
  last_machinery_state = !last_machinery_state;
}

void KEY_up() { Light_off(ALL); }

void task_Key() _task_ 1 {

  while (1) {
    KEY_scan();
    os_wait2(K_TMO, 2);
  }
}

void task_Track() _task_ 2 {

  while (1) {
    u8 code states[5];
    char i;
    int pos;

    Track_get_state(&states);
    for (i = 4; i >= 0; i--) {
      printf("%d ", (int)states[i]);
    }
    printf("\n");

    pos = Track_get_position();
    printf("pos: %d\n", pos);

    os_wait2(K_TMO, 40); // 5 * 40 = 200ms,ÿ200ms��ȡһ��
  }
}