#ifndef __STRING_H
#define __STRING_H

/* copying */
extern unsigned char *memcpy(void *dest, const void *src, int count);

/* other */
extern unsigned char *memset(void *dest, int val, int count);
extern unsigned short *memsetw(void *dest, short int val, int count);
extern int strlen(const char *str);

#endif
