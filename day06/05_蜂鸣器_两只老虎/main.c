#include "Config.h"
#include "Delay.h"
#include "Buzzer.h"

/*下面的数组根据实际的音谱进行编写即可*/
// 音符数组
u8 code notes[] = {
	1, 2, 3, 1,					1, 2, 3, 1,					3, 4, 5,		3, 4, 5,
	5, 6, 5, 4,	3, 1, 	5, 6, 5, 4,	3, 1, 	1, 5, 1,		1, 5, 1,
};

// 延时时长
u8 code durations[] = {
	4, 4, 4, 4,					4, 4, 4, 4,					4, 4, 8,		4, 4, 8,
	3, 1, 3, 1, 4, 4, 	3, 1, 3, 1, 4, 4, 	4, 4, 8,		4, 4, 8
};

int main() {
		u8 i =0;
    u8 len = 0;
    u16 delay;

    Buzzer_init();
    EA = 1;//开启全局中断
		len = sizeof(notes) / sizeof(u8);
    while (1) {
        for (i = 0; i < len; ++i){
            //要播放的音符
            Buzzer_beep(notes[i]);
            //要播放的时长
            delay = durations[i] * 100;
            delay_X_ms(delay);

            //稍微停止一下蜂鸣器
            Buzzer_stop();
						delay_ms(20);
        }
				Buzzer_stop();
				
        delay_X_ms(1000);
    }
}