/*
 * Jiauliyan OS - Released under the MIT License
 * Copyright (C) 2011 Paul Booth, Jialiya Huang, Tim Ryan
 * https://github.com/timcameronryan/jiauliyan
 * 
 * Based on code from Bran's Kernel Development Tutorial
 * http://www.osdever.net/tutorials/view/brans-kernel-development-tutorial
 */

#ifndef __TIMER_H
#define __TIMER_H

extern void timer_wait(unsigned int ticks);
extern void timer_install();
extern void timer_phase(int hz);

#endif
