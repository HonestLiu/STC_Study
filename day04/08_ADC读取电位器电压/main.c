#include "Config.h"
#include "Delay.h"
#include "GPIO.h"

#include "ADC.h"
#include "NVIC.h"
#include "Switch.h"
#include "UART.h"

/**
ʹ��ADC��ȡP05�Ĳ���ֵ
*/

// ��ʼ��GPIO
void gpio_config(void) {
  // ADC
  P0_MODE_IN_HIZ(GPIO_Pin_5); // ����P05,�������룬����ADC�źŵĲɼ�
}

/*��ʼ��UART*/
void uart_config(void) {
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

// ��ʼ��ADC
void ADC_config(void) {
  ADC_InitTypeDef ADC_InitStructure; // �ṹ����

  ADC_InitStructure.ADC_SMPduty = 31; // ADC ģ���źŲ���ʱ�����, 0~31��ע�⣺
                                      // SMPDUTY һ����������С�� 10��
  ADC_InitStructure.ADC_CsSetup = 0; // ADC ͨ��ѡ��ʱ����� 0(Ĭ��),1
  ADC_InitStructure.ADC_CsHold = 1; // ADC ͨ��ѡ�񱣳�ʱ����� 0,1(Ĭ��),2,3
  ADC_InitStructure.ADC_Speed =
      ADC_SPEED_2X1T; // ���� ADC ����ʱ��Ƶ��	ADC_SPEED_2X1T~ADC_SPEED_2X16T
  ADC_InitStructure.ADC_AdjResult =
      ADC_RIGHT_JUSTIFIED;         // ADC�������,
                                   // ADC_LEFT_JUSTIFIED,ADC_RIGHT_JUSTIFIED
  ADC_Inilize(&ADC_InitStructure); // ��ʼ��
  ADC_PowerControl(ENABLE);        // ADC��Դ����, ENABLE��DISABLE
  NVIC_ADC_Init(DISABLE,
                Priority_0); // �ж�ʹ��, ENABLE/DISABLE; ���ȼ�(�͵���)
                             // Priority_0,Priority_1,Priority_2,Priority_3
}

int main() {
  u16 adc_value;    // ADC�Ĳ���ֵ���Ϊ12λ(0~4095)
  u16 adc_value_15; // �ɼ�ADC15ͨ����ADC�ź�
  float VRef;       // ͨ��ADCת��������Ƶĵ�ѹֵ
  float VRef_15;    // ͨ��ADCת��������Ƶ�ADC15ͨ���ĵ�ѹֵ

  uart_config();
  gpio_config();
  ADC_config();

  EA = 1; // ʹ��ȫ���ж�

  while (1) {
    // ��ȡADCת�����
    adc_value = Get_ADCResult(
        ADC_CH13); // ͨ���Ǹ���ʵ��ʹ�õĶ˿ڶ����ģ�����P05��Ӧ����CH13
    adc_value_15 = Get_ADCResult(ADC_CH15);

    // ͨ����ʽ���Ƴ�ADC�����ѹ(������ѹ)
    VRef = 2.5 * adc_value / 4096;
    VRef_15 = 2.5 * adc_value / 4096;

    // ��ӡ�������
    printf("ADC:%d VRef:%f\n", adc_value, VRef);
    printf("ADC_15:%d VRef_15:%f\n", adc_value_15, VRef_15);

    delay_ms(250);
    delay_ms(250);
    delay_ms(250);
    delay_ms(250);
  }
}
