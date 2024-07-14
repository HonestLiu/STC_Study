#include "../Drive/Light.h"
#include "Config.h"
#include "GPIO.h"
#include "RTX51TNY.h" //RTX51的系统头文件

void sys_init() {
  EAXSFR();

  Light_init();

  EA = 1;
}

void start() _task_ 0 {
  sys_init();
  os_create_task(1); // 创建任务1
                     // os_create_task(2);//创建任务2

  // 销毁自己
  os_delete_task(0);
}

void task1() _task_ 1 {

  while (1) {
    // 依次点亮左右灯
    Light_on(LEFT);
    os_wait2(K_TMO, 100);
    Light_on(RIGHT);
    os_wait2(K_TMO, 100);

    // 全部熄灭
    Light_off(ALL);
    os_wait2(K_TMO, 100);

    // 全部点亮
    Light_on(ALL);
    os_wait2(K_TMO, 100);

    // 全部熄灭
    Light_off(ALL);
    os_wait2(K_TMO, 100);
  }
}

/* void task2() _task_ 2 {

  while (1) {

  }
} */