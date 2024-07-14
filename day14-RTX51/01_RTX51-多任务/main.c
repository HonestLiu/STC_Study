#include "RTX51TNY.H" //RTX51的系统头文件
#include "STC8H.h"

void start() _task_ 0 {
  os_create_task(1); // 创建任务1
  os_create_task(2);//创建任务2
  
  os_delete_task(0);//销毁自己
}

void task1() _task_ 1 {
  // 不依赖GPIO头文件初始化P5管脚
  P5M1 = 0;
  P5M0 = 1;

  while (1) {
    P53 = !P53;
    // 不占用系统资源, 告诉操作系统,
    // 当前任务在等待(在24M运行频率下，200正好对应1000ms)
    os_wait2(K_TMO, 200); // 5ms * 200 = 1000ms (24M)
  }
}

void task2() _task_ 2 {
  P4M0 = 0;
  P4M1 = 0x00;

  while (1) {
    P45 = !P45;
    os_wait2(K_TMO, 200);
  }
}