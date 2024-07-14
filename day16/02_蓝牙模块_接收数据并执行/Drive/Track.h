#ifndef __TRACK_H__
#define __TRACK_H__

#include "../Lib/Config.h"

#define TRACK_0 P47
#define TRACK_1 P06
#define TRACK_2 P32
#define TRACK_3 P33
#define TRACK_4 P50

void Track_init();
void Track_get_state(u8 states[]);
int Track_get_position();

#endif // !__TRACK_H__