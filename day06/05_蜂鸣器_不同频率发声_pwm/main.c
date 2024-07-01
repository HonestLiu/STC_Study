#include "Config.h"
#include "Delay.h"
#include "Buzzer.h"


int main() {
    u8 idx = 1;
    Buzzer_init();
    EA = 1;//开启全局中断
    while (1) {
				//传入音阶控制蜂鸣器响声
        Buzzer_beep(idx);
        if (++idx > 8) {
            idx = 1;
        }
        delay_ms(250);
        delay_ms(250);
        delay_ms(250);
        delay_ms(250);
    }
}