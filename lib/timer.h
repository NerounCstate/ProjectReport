#ifndef _TIMER_H
#define _TIMER_H

#include <8052.h>
#include "public.h"

// 函数声明
void timer0_init(void);
void timer0_isr(void) __interrupt(1);
void time_control(void);
u16 get_minutes(void);
u16 get_seconds(void);
void set_time(u16 m, u16 s);
u8 is_countdown_active(void);
void play_mi_sound(void);

#endif