#ifndef __VGA_H
#define __VGA_H

#include <stream.h>

extern void vga_cls();
extern void vga_placechar(char c, int x, int y);
extern void vga_putchar(char c);
extern void vga_setbg(unsigned char bg);
extern void vga_setfg(unsigned char fg);
extern void vga_init();

extern stream_s *vgaout;

#define BLACK 0
#define BLUE 1
#define GREEN 2
#define CYAN 3
#define RED 4
#define MAGENTA 5
#define BROWN 6
#define LIGHT_GREY 7
#define DARK_GREY 8
#define LIGHT_BLUE 9
#define LIGHT_GREEN 10
#define LIGHT_CYAN 11
#define LIGHT_RED 12
#define LIGHT_MAGENTA 13
#define LIGHT_BROWN 14
#define WHITE 15

#define SCREEN_WIDTH 80
#define SCREEN_HEIGHT 25

#endif
