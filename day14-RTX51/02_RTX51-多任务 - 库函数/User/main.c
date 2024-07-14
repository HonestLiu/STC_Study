#include "RTX51TNY.h" //RTX51��ϵͳͷ�ļ�
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
  os_create_task(1); // ��������1
	os_create_task(2);//��������2
	
	// �����Լ�
		os_delete_task(0);
}

void task1() _task_ 1 {

  while (1) {
    P10 = !P10;
    // ��ռ��ϵͳ��Դ, ���߲���ϵͳ,
    // ��ǰ�����ڵȴ�(��24M����Ƶ���£�200���ö�Ӧ1000ms)
    os_wait2(K_TMO, 200); // 5ms * 200 = 1000ms (24M)
  }
}

void task2() _task_ 2 {

  while (1) {
    P11 = !P11;
    os_wait2(K_TMO, 200);
  }
}