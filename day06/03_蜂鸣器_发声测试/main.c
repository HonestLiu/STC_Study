#include "Config.h"
#include "GPIO.h"
#include "Delay.h"

#define BuzzerPin P00

void gpio_config(void) {
  // 初始化NPN S8050的控制引脚 - P00
  P0_MODE_OUT_PP(GPIO_Pin_0);//推挽输出
}


int main() {
    gpio_config();
  
    while (1){
			BuzzerPin = 1;
			delay_ms(2);
			BuzzerPin = 0;
			delay_ms(2);
		}
       
}