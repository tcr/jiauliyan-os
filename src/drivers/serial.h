/*
 * Jiauliyan OS - Released under the MIT License
 * Copyright (C) 2011 Paul Booth, Jialiya Huang, Tim Ryan
 * https://github.com/timcameronryan/jiauliyan
 * 
 * Based on code from the OSDev wiki
 * http://wiki.osdev.org/Serial_ports
 */

#ifndef __SERIAL_H
#define __SERIAL_H

#include <stream.h>

extern void serial_install();
extern void serial_set_handler(void (*callback)(unsigned char *buf, long int size));
extern void serial_flush();
extern stream_s *serialout;
extern stream_s *serialin;

#endif
