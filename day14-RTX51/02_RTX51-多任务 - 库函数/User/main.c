#include "RTX51TNY.h" //RTX51的系统头文件
#include "Config.h"
#include "GPIO.h"

void GPIO_config(){
	GPIO_InitTypeDef gpio_init_struct;
	gpio_init_struct.Mode = GPIO_PullUp;
	gpio_init_struct.Pin = GPIO_Pin_0 | GPIO_Pin_1;
	GPIO_Inilize(GPIO_P1,&gpio_init_struct);
	
//	gpio_init_struct.Mode = GPIO_PullUp;
//	gpio_init_struct.Pin = GPIO_Pin_5;
//	GPIO_Inilize(GPIO_P4,&gpio_init_struct);
}

void sys_init(){
	EAXSFR();
	
	GPIO_config();
	
	EA = 1;
	
}

void start() _task_ 0 {
	sys_init();
  os_create_task(1); // 创建任务1
	os_create_task(2);//创建任务2
	
	// 销毁自己
		os_delete_task(0);
}

void task1() _task_ 1 {

  while (1) {
    P10 = !P10;
    // 不占用系统资源, 告诉操作系统,
    // 当前任务在等待(在24M运行频率下，200正好对应1000ms)
    os_wait2(K_TMO, 200); // 5ms * 200 = 1000ms (24M)
  }
}

void task2() _task_ 2 {

  while (1) {
    P11 = !P11;
    os_wait2(K_TMO, 200);
  }
}