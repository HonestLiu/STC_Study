#include "Buzzer.h"

void Delay100us(void) //@24.000MHz
{
  unsigned char data i, j;

  i = 3;
  j = 82;
  do {
    while (--j)
      ;
  } while (--i);
}

void buzzer_init() { BUZZER_IO_INIT(); }

void buzzer_beep() {
  int i = 1000;

  // 人为的生产周期为0.2ms为周期的PWM方波，对应频率就是1000 /0.2 = 5000
  while (i-- > 0) {
    BUZZER = !BUZZER;
    Delay100us();
  }
}