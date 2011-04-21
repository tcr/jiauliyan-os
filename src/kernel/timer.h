#ifndef __TIMER_H
#define __TIMER_H

extern void timer_wait(unsigned int ticks);
extern void timer_install();
extern void timer_phase(int hz);

#endif
