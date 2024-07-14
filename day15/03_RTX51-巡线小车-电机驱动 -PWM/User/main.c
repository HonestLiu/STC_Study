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
#include "../Lib/STC8H_PWM.h"
#include "../Lib/Switch.h"
#include "../Lib/UART.h"

#include "RTX51TNY.h" //RTX51��ϵͳͷ�ļ�

#define RQ_B P15
#define RQ_F P14

#define PERIOD (MAIN_Fosc / 1000)
PWMx_Duty dutyA;
void PWM_config(void) {
  PWMx_InitDefine PWMx_InitStructure;
  // ����PWM3
  PWMx_InitStructure.PWM_Mode =
      CCMRn_PWM_MODE1; // ģʽ,
                       // CCMRn_FREEZE,CCMRn_MATCH_VALID,CCMRn_MATCH_INVALID,CCMRn_ROLLOVER,CCMRn_FORCE_INVALID,CCMRn_FORCE_VALID,CCMRn_PWM_MODE1,CCMRn_PWM_MODE2
  PWMx_InitStructure.PWM_Duty =
      PERIOD * 50 / 100; // PWMռ�ձ�ʱ��->ռ�������ڵ�50%
  PWMx_InitStructure.PWM_EnoSelect =
      ENO3P |
      ENO3N; // ���ͨ��ѡ��,	ENO1P,ENO1N,ENO2P,ENO2N,ENO3P,ENO3N,ENO4P,ENO4N
             // / ENO5P,ENO6P,ENO7P,ENO8P
  PWM_Configuration(PWM3, &PWMx_InitStructure);

  // ����PWMA
  PWMx_InitStructure.PWM_Period = PERIOD - 1;    // ����ʱ��,   0~65535
  PWMx_InitStructure.PWM_DeadTime = 0;           // ��������������, 0~255
  PWMx_InitStructure.PWM_MainOutEnable = ENABLE; // �����ʹ��, ENABLE,DISABLE
  PWMx_InitStructure.PWM_CEN_Enable = ENABLE; // ʹ�ܼ�����, ENABLE,DISABLE
  PWM_Configuration(PWMA,
                    &PWMx_InitStructure); // ��ʼ��PWMͨ�üĴ���,  PWMA,PWMB

  // �л�PWMͨ��
  PWM3_SW(PWM3_SW_P14_P15); // PWM3_SW_P14_P15,PWM3_SW_P24_P25,PWM3_SW_P64_P65

  // ��ʼ��PWMA���ж�
  NVIC_PWM_Init(PWMA, DISABLE, Priority_0);
}

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
  PWM_config();

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

u8 d3 = 50; // �����ռ�ձ� 0 ~ 100
int direction_step = 10;
void KEY_down() {
  Light_on(ALL);
  // �������£�����ռ�ձ�
  d3 += direction_step;
  if (d3 == 100) {
    direction_step = -10; // ��ʼ�ݼ�
  } else if (d3 <= 0) {
    direction_step = 10; // ��ʼ����
  }
  dutyA.PWM3_Duty = PERIOD * d3 / 100;
  UpdatePwm(PWM3, &dutyA);
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