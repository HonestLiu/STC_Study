#include "../Drive/Light.h"
#include "Config.h"
#include "GPIO.h"
#include "RTX51TNY.h" //RTX51��ϵͳͷ�ļ�

void sys_init() {
  EAXSFR();

  Light_init();

  EA = 1;
}

void start() _task_ 0 {
  sys_init();
  os_create_task(1); // ��������1
                     // os_create_task(2);//��������2

  // �����Լ�
  os_delete_task(0);
}

void task1() _task_ 1 {

  while (1) {
    // ���ε������ҵ�
    Light_on(LEFT);
    os_wait2(K_TMO, 100);
    Light_on(RIGHT);
    os_wait2(K_TMO, 100);

    // ȫ��Ϩ��
    Light_off(ALL);
    os_wait2(K_TMO, 100);

    // ȫ������
    Light_on(ALL);
    os_wait2(K_TMO, 100);

    // ȫ��Ϩ��
    Light_off(ALL);
    os_wait2(K_TMO, 100);
  }
}

/* void task2() _task_ 2 {

  while (1) {

  }
} */