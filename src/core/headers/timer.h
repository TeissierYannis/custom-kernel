#ifndef __TIMER_H
#define __TIMER_H

extern void timer_wait(int ticks);
extern void timer_phase(int hz);
extern void timer_handler(struct regs *r);
extern void timer_install();
extern void get_time(char* buffer);

#endif