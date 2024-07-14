#include "Key.h"

#define DOWN 1
#define UP 0

u8 key_last_state = UP;

void KEY_init(void) { KEY_IO_INIT(); }

void KEY_scan(void) {
    // 当按键状态不等于上一次按键状态时触发相关函数
  if (KEY != key_last_state) {
    // 更新按键状态
    key_last_state = KEY;
    // 根据按键状态触发相关函数
    if (KEY == DOWN) {
      KEY_down();
    } else{
      KEY_up();
    }
  }
}

//void KEY_down();
//void KEY_up();