/*
 * Jiauliyan OS - Released under the MIT License
 * Copyright (C) 2011 Paul Booth, Jialiya Huang, Tim Ryan
 * https://github.com/timcameronryan/jiauliyan
 */

#include <system.h>
#include <common.h>
#include <string.h>
#include <vga.h>

#define KEYBOARD_BUF_SIZE 1024*4

/* US Keyboard Layout. This is a scancode table
 * used to layout a standard US keyboard. I have left some
 * comments in to give you an idea of what key is what, even
 * though I set it's array index to 0. You can change that to
 * whatever you want using a macro, if you wish! */
int shift_pressed = 0, capslock = 0;
char kbdus[256] =
{
	0,  27, '1', '2', '3', '4', '5', '6', '7', '8',	/* 9 */
	'9', '0', '-', '=', '\b',	/* Backspace */
	'\t',			/* Tab */
	'q', 'w', 'e', 'r',	/* 19 */
	't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',	/* Enter key */
	0,			/* 29   - Control */
	'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';',	/* 39 */
	'\'', '`',   0,		/* Left shift */
	'\\', 'z', 'x', 'c', 'v', 'b', 'n',			/* 49 */
	'm', ',', '.', '/',   0,				/* Right shift */
	'*',
	0,	/* Alt */
	' ',	/* Space bar */
	0,	/* Caps lock */
	0,	/* 59 - F1 key ... > */
	0,   0,   0,   0,   0,   0,   0,   0,
	0,	/* < ... F10 */
	0,	/* 69 - Num lock*/
	0,	/* Scroll Lock */
	0,	/* Home key */
	0,	/* Up Arrow */
	0,	/* Page Up */
	'-',
	0,	/* Left Arrow */
	0,
	0,	/* Right Arrow */
	'+',
	0,	/* 79 - End key*/
	0,	/* Down Arrow */
	0,	/* Page Down */
	0,	/* Insert Key */
	0,	/* Delete Key */
	0,   0,   0,
	0,	/* F11 Key */
	0,	/* F12 Key */
	0,	/* All other keys are undefined */
	/* 90 through 128 undefined */
/* SHIFT VALUES add 90 to get to shift value */
	0,  27, '!', '@', '#', '$', '%', '^', '&', '*',	/* 9 */
	'(', ')', '_', '+', '\b',	/* BACKSPACE */
	'\t',			/* TAB */
	'Q', 'W', 'E', 'R',	/* 19 */
	'T', 'Y', 'U', 'I', 'O', 'P', '{', '}', '\n',	/* ENTER KEY */
	0,			/* 29   - CONTROL */
	'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ':',	/* 39 */
	'"', '~',   0,		/* LEFT SHIFT */
	'|', 'Z', 'X', 'C', 'V', 'B', 'N',			/* 49 */
	'M', '<', '>', '?',   0,			/* RIGHT SHIFT */
	'*',
	0,	/* ALT */
	' ',	/* SPACE BAR */
	0,	/* CAPS LOCK */
	0,	/* 59 - F1 KEY ... > */
	0,   0,   0,   0,   0,   0,   0,   0,
	0,	/* < ... F10 */
	0,	/* 69 - NUM LOCK*/
	0,	/* SCROLL LOCK */
	0,	/* HOME KEY */
	0,	/* UP ARROW */
	0,	/* PAGE UP */
	'-',
	0,	/* LEFT ARROW */
	0,
	0,	/* RIGHT ARROW */
	'+',
	0,	/* 79 - END KEY*/
	0,	/* DOWN ARROW */
	0,	/* PAGE DOWN */
	0,	/* INSERT KEY */
	0,	/* DELETE KEY */
	0,   0,   0,
	0,	/* F11 KEY */
	0,	/* F12 KEY */
	0,	/* ALL OTHER KEYS ARE UNDEFINED */
};

/*
 * interrupt/buffer
 */

unsigned char keyboard_buf[KEYBOARD_BUF_SIZE];
int keyboard_buf_len = 0;
void (*keyboard_handler)(unsigned char *buf, long int size) = NULL;

void keyboard_set_handler(void (*callback)(unsigned char *buf, long int size))
{
	keyboard_handler = callback;
}

void keyboard_flush()
{
	if (keyboard_buf_len > 0) {
		disable_interrupts();
		if (keyboard_handler != NULL)
			keyboard_handler(keyboard_buf, keyboard_buf_len);
		keyboard_buf_len = 0;
		enable_interrupts();
	}
}

#include <serial.h>

void keyboard_interrupt(struct regs *r)
{
	(void) r;
	
    unsigned char scancode;

    /* Read from the keyboard's data buffer */
    scancode = inportb(0x60);

    /* If the top bit of the byte we read from the keyboard is
    *  set, that means that a key has just been released */
    if (scancode & 0x80)
    {
        /* You can use this one to see if the user released the
        *  shift, alt, or control keys... */
	   
	    /* left shift */
	    if (((scancode & 0x7F) == 42) || ((scancode & 0x7F) == 54)) {
		    shift_pressed--;
	    } 
    }
    else
    {
        /* Here, a key was just pressed. Please note that if you
        *  hold a key down, you will get repeated key press
        *  interrupts. */

        /* Just to show you how this works, we simply translate
        *  the keyboard scancode into an ASCII value, and then
        *  display it to the screen. You can get creative and
        *  use some flags to see if a shift is pressed and use a
        *  different layout, or you can add another 128 entries
        *  to the above layout to correspond to 'shift' being
        *  held. If shift is held using the larger lookup table,
        *  you would add 128 to the scancode when you look for it */
        /* left or right shift pressed! */
	    /*printf("|%d|", scancode);*/
	    if ((scancode == 42) || (scancode == 54)){
		    shift_pressed++;
	    } else if (scancode == 58) {
		    capslock = !capslock;
		    shift_pressed += -1 + 2 * (capslock);
	    } else if (scancode == 14) {
		    keyboard_buf[keyboard_buf_len++] = '\b';
		    vga_putchar('\b');
		    vga_putchar(' ');
		    vga_putchar('\b');
	    } else {
		    keyboard_buf[keyboard_buf_len++] = kbdus[scancode + 
							     (shift_pressed > 0)
							     * 90]; 
/* 90 because that is the length of our kbdus array without shift. 
the rest to 128 are undefined. */
		    /* display to screen */
		    vga_putchar(kbdus[scancode + (shift_pressed > 0) * 90]);

		    if (keyboard_buf_len == KEYBOARD_BUF_SIZE)
			    keyboard_flush();
	    }
    }
}

/*
 * keyboard stream implementation
 */

int keyboardin_get(stream_s *stream)
{
	UNUSED(stream);	
	// read from serial buffer
	while (keyboard_buf_len == 0) { ; }
	unsigned char c = keyboard_buf[0];
	memcpy(keyboard_buf, keyboard_buf + 1, --keyboard_buf_len);
	return c;
}

long int keyboardin_avail(stream_s *stream)
{
	UNUSED(stream);
	return keyboard_buf_len;
}

stream_s *keyboardin;

/*
 * install
 */

void keyboard_install()
{
    // installs keyboard interrupt handler to IRQ1
    irq_install_handler(1, keyboard_interrupt);
    
    // create keyboard stream
    keyboardin = stream_create(keyboardin_get, stream_no_put, keyboardin_avail, stream_no_seek, NULL);
}
