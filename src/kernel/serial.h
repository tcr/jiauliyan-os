#ifndef __SERIAL_H
#define __SERIAL_H

#include <stream.h>

extern void serial_install();
extern void serial_set_handler(void (*callback)(unsigned char *buf, long int size));
extern void serial_flush();
extern stream_s *serialstream;

#endif
