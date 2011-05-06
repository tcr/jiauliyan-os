/*
 * Jiauliyan OS - Released under the MIT License
 * Copyright (C) 2011 Paul Booth, Jialiya Huang, Tim Ryan
 * https://github.com/timcameronryan/jiauliyan
 */

#ifndef __MEMORY_H
#define __MEMORY_H

#include <common.h>
#include <multiboot.h>

extern unsigned char *memory_ptr();
extern uint64_t memory_size();
extern uint64_t memory_init(multiboot_info_t *mbt);

#endif
