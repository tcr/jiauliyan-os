/*
 * Jiauliyan OS - Released under the MIT License
 * Copyright (C) 2011 Paul Booth, Jialiya Huang, Tim Ryan
 * https://github.com/timcameronryan/jiauliyan
 */
 
#ifndef __COMMON_H
#define __COMMON_H

#define UNUSED(a) (void) a

#define DEBUG(s) (void)
#define WARN(s) puts(s)
#define FATAL(s) stream_putstring(vgaout, s)

#endif
