#ifndef __TIMER_H__
#define __TIMER_H__

#include <8052.h>
#include "public.h"

// 外部全局变量声明
extern u16 minutes;
extern u16 seconds;
extern u8 countdown_active;

// 函数声明
void timer0_init(void);
void timer0_isr(void) __interrupt(1);
void time_control(void);
void play_mi_sound(void);
u16 get_minutes(void);
u16 get_seconds(void);
void set_time(u16 m, u16 s);
u8 is_countdown_active(void);
u8 is_countdown_finished(void);
void reset_countdown_finished(void);

#endif