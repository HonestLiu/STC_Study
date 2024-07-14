// �ⲿ�豸
#include "../Drive/Key.h"
#include "../Drive/Light.h"
#include "../Drive/Motors.h"
#include "../Drive/Track.h"
#include "../Drive/Ultrasonic.h"
#include "../Drive/battery.h"
#include "../Drive/Buzzer.h"

// Ƭ������
#include "../Lib/Config.h"
#include "../Lib/GPIO.h"
#include "../Lib/NVIC.h"
#include "../Lib/Switch.h"
#include "../Lib/UART.h"

#include "RTX51TNY.h" //RTX51��ϵͳͷ�ļ�

static void GPIO_config() {
  GPIO_InitTypeDef gpio_init_struct;
  gpio_init_struct.Mode = GPIO_PullUp;
  gpio_init_struct.Pin = GPIO_Pin_1 | GPIO_Pin_0;
  GPIO_Inilize(GPIO_P1, &gpio_init_struct);
}

void UART_config(void) {

  /* UART1��ʼ�� ���� PC to MCU */
  COMx_InitDefine COMx_InitStructure;
  COMx_InitStructure.UART_Mode = UART_8bit_BRTx;
  COMx_InitStructure.UART_BRT_Use = BRT_Timer1;
  COMx_InitStructure.UART_BaudRate = 115200ul;
  COMx_InitStructure.UART_RxEnable = ENABLE;
  COMx_InitStructure.BaudRateDouble = DISABLE;
  UART_Configuration(UART1, &COMx_InitStructure);
  NVIC_UART1_Init(ENABLE, Priority_1);
  UART1_SW(UART1_SW_P30_P31);

  /* UART2��ʼ�� ���� MCU to BlueTooth */
  COMx_InitStructure.UART_Mode = UART_8bit_BRTx;
  COMx_InitStructure.UART_BRT_Use = BRT_Timer2;
  COMx_InitStructure.UART_BaudRate = 9600ul; // ������, ����ģ��Ĭ�Ͼ���9600?
  COMx_InitStructure.UART_RxEnable = ENABLE;
  COMx_InitStructure.BaudRateDouble = DISABLE;
  UART_Configuration(UART2, &COMx_InitStructure);
  NVIC_UART2_Init(ENABLE, Priority_1);
  UART2_SW(UART2_SW_P10_P11);
}

void sys_init() {
  EAXSFR();
  // Ƭ�������ʼ��
  UART_config(); // ��ʼ��UART
  GPIO_config();

  // ������ʼ��
  Light_init(); // ��ʼ��ת���
  KEY_init();   // ��ʼ����ť
  Battery_init();    // ��ʼ��ADC��Դ���
  buzzer_init();     // ��ʼ��������
  Ultrasonic_init(); // ��ʼ��������ģ��
  Track_init();      // ��ʼ��Ѳ��ģ��
  Motor_init();      // ��ʼ���������

  EA = 1;
}

#define UART1_TASK 2
#define UART2_TASK 3

void start_main() _task_ 0 {
  sys_init();
  os_create_task(1); // ������������
  os_create_task(UART1_TASK);
  os_create_task(UART2_TASK);

  // �����Լ�
  os_delete_task(0);
}

u8 track_state = 0; // Ѳ�߹���״̬ 0��δ���� 1:������
void KEY_down() { Light_on(ALL); }

void KEY_up() { Light_off(ALL); }

void task_Key() _task_ 1 {

  while (1) {
    KEY_scan();
    os_wait2(K_TMO, 2);
  }
}

// ��PC�˽������ݣ��������ݷ��͸�����ģ��
void task_uart1() _task_ UART1_TASK {
  u8 i;
  while (1) {
    if (COM1.RX_TimeOut > 0) {
      // ��ʱ����
      if (--COM1.RX_TimeOut == 0) {
        if (COM1.RX_Cnt > 0) {
          // ���ﴦ���յ������ݣ���������߼������Ե����Լ���on_uart1_recv
          for (i = 0; i < COM1.RX_Cnt; i++) {
            // д�����ݵ�����
            TX2_write2buff(RX1_Buffer[i]);
            TX1_write2buff(RX1_Buffer[i]);
          }
        }
        COM1.RX_Cnt = 0;
      }
    }
    os_wait2(K_TMO, 2);
  }
}

void bt_controller() {
  u8 dat = RX2_Buffer[0];
  
  if (dat == 0x01) {//ǰ��
    Motor_forward(70);
  }else if (dat == 0x02)//����
  {
    Motor_backward(70);
  }else if (dat == 0x03)
  { 
    Motor_stop();
  }else if (dat == 0x04)//����
  {
    Light_on(ALL);
    buzzer_beep();
  }else if (dat == 0x05)//�ص�
  {
    Light_off(ALL);
    buzzer_beep();
  }
}

// ��������ģ�鷢�ͻ��������ݣ��������ݷ��͸�PC
void task_uart2() _task_ UART2_TASK {
  u8 i;
  while (1) {
    if (COM2.RX_TimeOut > 0) {
      // ��ʱ����
      if (--COM2.RX_TimeOut == 0) {
        if (COM2.RX_Cnt > 0) {
          bt_controller();
          for (i = 0; i < COM2.RX_Cnt; i++) {
            // д�����ݵ�PC
            TX1_write2buff(RX2_Buffer[i]);
          }
        }
        COM2.RX_Cnt = 0;
      }
    }
    os_wait2(K_TMO, 2);
  }
}
