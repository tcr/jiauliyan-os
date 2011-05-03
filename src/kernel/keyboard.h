/*
 * Jiauliyan OS - Released under the MIT License
 * Copyright (C) 2011 Paul Booth, Jialiya Huang, Tim Ryan
 * https://github.com/timcameronryan/jiauliyan
 * 
 * Based on code from Bran's Kernel Development Tutorial
 * http://www.osdever.net/tutorials/view/brans-kernel-development-tutorial
 */

#ifndef __KEYBOARD_H
#define __KEYBOARD_H

#include <stream.h>

extern void keyboard_install();
extern void keyboard_set_handler(void (*callback)(unsigned char *buf, long int size));
extern void keyboard_flush();
extern stream_s *keyboardin;

#endif
