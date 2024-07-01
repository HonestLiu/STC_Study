#include "Config.h"
#include "Delay.h"
#include "NIXIE.h"


// 记录字母位(控制要显示什么内容)
u8 letter = 0x02; // 数字6 -> 0000 0010
// 记录数字位(控制哪个数码管显示内容)
u8 digital = 0x0A; // 0000 1010(左侧两个数码管点亮)

u8 i;

int main(void) {
  nixie_init();
  while (1) {
    //让数码管“同时”显示12345678(通过快速切换实现，实际一瞬间只亮了一个)
    for (i = 0; i < 8; i++) {
      // 快速切换显示
      nixie_display(i + 1, i);
      delay_ms(2);
    }
  }
}