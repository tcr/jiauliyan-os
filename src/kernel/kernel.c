#include <system.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <setjmp.h>

#include <vga.h>
#include <serial.h>
#include <keyboard.h>

#include "../../lua-5.1/src/lua.h"
#include "../../lua-5.1/src/lualib.h"
#include "../../lua-5.1/src/lauxlib.h"

extern int _binary_os_lua_start;
extern int _binary_os_lua_size;

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

void kernel_start()
{
	serial_set_handler(kernel_serial_handler); // serial buffer handler
	keyboard_set_handler(kernel_keyboard_handler); // keyboard buffer handler
	
	vga_setbg(BLUE);
	vga_cls();
	
	vga_setfg(LIGHT_BROWN);
	puts("Jiauliyan OS\n");
	vga_setfg(WHITE);
	puts("An OS by some cool dudes\n\n");

	puts("Address of kernel_start(): ");
	stream_putp(vgastream, &kernel_start);
	putchar('\n');

	puts("Writing to serial port...\n");
	stream_puts(serialstream, "[SERIAL] Testing serial ports from Jiauliyan OS!\n");

	/*
	puts("Waiting timer for 100 clicks:\n");
	timer_wait(100);	
	puts("100 click timer DONE!\n");
	putchar('\n');
	*/
	
	/*
	puts("Testing input. Serial input is ");
	vga_setfg(LIGHT_RED);
	puts("red");
	vga_setfg(WHITE);
	puts(" and keyboard input is ");
	vga_setfg(LIGHT_GREEN);
	puts("green");
	vga_setfg(WHITE);
	puts(".\n");
	*/
	
	int size = (int)&_binary_os_lua_size;
    char *data = (char *)&_binary_os_lua_start;
    
    puts(data);
	
	FILE *f = fopen("hello.lua", "w");
	fputs(data, f);
	fclose(f);
	
	lua_State* l;
	int dofile;
	/* initialize lua */
	l = lua_open();
	/* load lua libraries */
	luaL_openlibs(l);
	//// run the hello.lua script 
	dofile = luaL_dofile(l, "hello.lua");
	if (dofile == 0) {
		// call foo
		lua_getglobal(l,"foo");
		lua_call(l,0,0);
	} else {
		
	stream_puts(vgastream, "Error: ");
	stream_puti(vgastream, dofile);
		puts( "\nError, unable to run hello.lua\n");
	}
	// cleanup Lua
	lua_close(l);
	puts("Done with lua");

	/* Write your kernel here. */
	for(;;) {
		keyboard_flush();
		serial_flush();
	}
}

void kmain( void* mbd, unsigned int magic )
{
	if ( magic != 0x2BADB002 )
	{
		/* Something went not according to specs. Print an error */
		/* message and halt, but do *not* rely on the multiboot */
		/* data structure. */
	}
	
	/* You could either use multiboot.h */
	/* (http://www.gnu.org/software/grub/manual/multiboot/multiboot.html#multiboot_002eh) */
	/* or do your offsets yourself. The following is merely an example. */ 
	char *boot_loader_name = (char *) ((long *) mbd)[16];
	(void) boot_loader_name; // use this eventually
	
	// global interrupts table
    gdt_install();
    idt_install();
    isrs_install();
    irq_install();
    
    // services
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
