/*
 * Jiauliyan OS - Released under the MIT License
 * Copyright (C) 2011 Paul Booth, Jialiya Huang, Tim Ryan
 * https://github.com/timcameronryan/jiauliyan
 */

#include <common.h>
#ifndef MULTIBOOT_HEADER_MAGIC
#include <multiboot.h>
#endif
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

static unsigned char *ptr = NULL;
static uint64_t size = 0;

unsigned char *memory_ptr()
{
	return ptr;
}

uint64_t memory_size()
{
	return size;
}

uint64_t memory_init(multiboot_info_t *mbt)
{
	/* search multiboot structure for largest value */
	multiboot_memory_map_t *mmap = (multiboot_memory_map_t *) mbt->mmap_addr;
	while((unsigned long) mmap < mbt->mmap_addr + mbt->mmap_length) {
		mmap = (multiboot_memory_map_t *) ((unsigned int) mmap + mmap->size + sizeof(unsigned int));
		if (mmap->len > size) {
			size = mmap->len;
			ptr = (void *) ((uint32_t) mmap->addr);
		}
	}

	return size;
}
