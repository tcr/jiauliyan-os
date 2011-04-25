#ifndef __KEYBOARD_H
#define __KEYBOARD_H

#include <stream.h>

extern void keyboard_install();
extern void keyboard_set_handler(void (*callback)(unsigned char *buf, long int size));
extern void keyboard_flush();
extern stream_s *keyboardin;

#endif
