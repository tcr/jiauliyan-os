/*
 * Jiauliyan OS - Released under the MIT License
 * Copyright (C) 2011 Paul Booth, Jialiya Huang, Tim Ryan
 * https://github.com/timcameronryan/jiauliyan
 * 
 * Based on code from Bran's Kernel Development Tutorial
 * http://www.osdever.net/tutorials/view/brans-kernel-development-tutorial
 */

#include <common.h>
#include <multiboot.h>
#include <system.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <setjmp.h>

#include <vga.h>
#include <serial.h>
#include <keyboard.h>
#include <timer.h>
#include <memory.h>

#include "../../lua-5.1/src/lua.h"
#include "../../lua-5.1/src/lualib.h"
#include "../../lua-5.1/src/lauxlib.h"
#include <lua_kernel.h>

extern int _binary_os_lua_start;
extern int _binary_os_lua_size;
extern int _binary_dkjson_lua_start;
extern int _binary_dkjson_lua_size;

void kernel_serial_handler(unsigned char *buf, long int size)
{
	vga_setfg(LIGHT_RED);
	int i;
	for (i = 0; i < size; i++)
		vga_putchar((char) buf[i]);
}

void kernel_keyboard_handler(unsigned char *buf, long int size)
{
	vga_setfg(LIGHT_GREEN);
	int i;
	for (i = 0; i < size; i++)
		vga_putchar((char) buf[i]);
}

void load_file_data(char *name, char *data, int data_size)
{
	FILE *f = fopen(name, "w");
	fwrite(data, 1, data_size, f);
	fclose(f);
}

void kernel_start()
{	
	vga_setbg(DARK_GREY);
	vga_cls();
	
	vga_setfg(LIGHT_RED);
	puts("Jiauliyan OS\n");
	vga_setfg(LIGHT_BROWN);
	puts("An OS by some cool dudes\n\n");

	puts("Address of kernel_start(): ");
	stream_putpointer(vgaout, &kernel_start);
	putchar('\n');
	printf("Memory start: %p (%d Mb)\n", memory_ptr(), (int) memory_size()/1024/1024);

	/*
	puts("Testing write to serial port...\n");
	stream_putstring(serialout, "[SERIAL] Testing serial ports from Jiauliyan OS!\n");
	*/

	/*
	puts("Waiting timer for 100 clicks:\n");
	timer_wait(100);	
	puts("100 click timer DONE!\n");
	putchar('\n');
	*/
	
	load_file_data("os.lua", (char *) &_binary_os_lua_start, (int) &_binary_os_lua_size);
	load_file_data("dkjson.lua", (char *) &_binary_dkjson_lua_start, (int) &_binary_dkjson_lua_size);
	
    puts("Starting lua.\n\n");
    vga_setfg(WHITE);
    
/**************************/
	
	lua_State* l;
	int dofile;
	// initialize lua
	l = lua_open();
	// load lua libraries
	luaL_openlibs(l);
	luaopen_kernel(l);
	
	dofile = luaL_dofile(l, "dkjson.lua");
	if (dofile != 0) {
		fprintf(stderr, "Unable to run dkjson.lua: %s\n", lua_tostring(l, -1));
		exit(EXIT_FAILURE);
	}
	
	dofile = luaL_dofile(l, "os.lua");
	if (dofile != 0) {
		fprintf(stderr, "Unable to run os.lua: %s\n", lua_tostring(l, -1));
		exit(EXIT_FAILURE);
	}
	
	// start command line
	lua_getglobal(l, "cli");
	lua_call(l, 0, 0);
	
	// cleanup Lua
	lua_close(l);
	
/***************************/

	vga_setfg(LIGHT_GREEN);
	puts("\nLua execution finished.\n\n");
	
	return;
	
	vga_setfg(WHITE);
	puts("Testing input. Serial input is ");
	vga_setfg(LIGHT_RED);
	puts("red");
	vga_setfg(WHITE);
	puts(" and keyboard input is ");
	vga_setfg(LIGHT_GREEN);
	puts("green");
	vga_setfg(WHITE);
	puts(".\n");
	
	serial_set_handler(kernel_serial_handler); // serial buffer handler
	//keyboard_set_handler(kernel_keyboard_handler); // keyboard buffer handler

	/* Write your kernel here. */
	for(;;) {
		//keyboard_flush();
		serial_flush();
	}
}

void kmain(multiboot_info_t *mbt, unsigned int magic)
{
	if ( magic != 0x2BADB002 )
	{
		/* Something went not according to specs. Can't rely on multiboot structure. */
		FATAL("Multiboot header was invalid.");
		return;
	}
	
	// global interrupts table
    gdt_install();
    idt_install();
    isrs_install();
    irq_install();

    // drivers
	memory_init(mbt);
    vga_init();
    timer_install();
    timer_phase(100);
    keyboard_install();
	serial_install();
	enable_interrupts();
	
	// init std(in|out|err)
	stdio_init();
	
	// start kernel
	kernel_start();
}
