#ifndef __COMMON_H
#define __COMMON_H

#define UNUSED(a) (void) a

#define DEBUG(s) (void)
#define WARN(s) puts(s)
#define FATAL(s) stream_putstring(vgaout, s)

#endif
