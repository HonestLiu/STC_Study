#include "../Drive/Key.h"
#include "../Drive/Light.h"
#include "../Drive/battery.h"
#include "Config.h"
#include "GPIO.h"
#include "RTX51TNY.h" //RTX51��ϵͳͷ�ļ�

void sys_init() {
  EAXSFR();

  Light_init();//��ʼ��ת���
  KEY_init();//��ʼ����ť
  Battery_init();//��ʼ��ADC��Դ���

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
  float voltage;
  Light_on(ALL);
  Battery_get_voltage(&voltage);
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