#include "../Drive/Key.h"
#include "../Drive/Light.h"
#include "Config.h"
#include "GPIO.h"
#include "RTX51TNY.h" //RTX51的系统头文件

void sys_init() {
  EAXSFR();

  Light_init();
  KEY_init();

  EA = 1;
}

void start() _task_ 0 {
  sys_init();
  os_create_task(1); // 创建任务1
                     // os_create_task(2);//创建任务2

  // 销毁自己
  os_delete_task(0);
}

void KEY_down() { Light_on(ALL); }

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