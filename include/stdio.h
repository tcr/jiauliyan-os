#ifndef __STDIO_H
#define __STDIO_H

#include <stream.h>

/* FILE */

typedef struct {
	stream_s *stream;		// stream implementation
	long pos;
	
	unsigned int flags; 	// file status flags
	unsigned int istemp;	// temporary file indicator
	
	unsigned char *buffer;	// data transfer buffer (min size 256)
	int bsize;				// buffer size
	unsigned char hold;		// ungetc char if no buffer
} FILE;

/* stdio */

extern int fputc(int character, FILE *file);
#define putc(C, S) fputc(C, S)
#define putchar(C) fputc(C, stdout)
extern int fputs(const char *str, FILE *file);
#define puts(C) fputs(C, stdout)

/* std(in|out|err) */

extern FILE *stdout;
extern FILE *stdin;
extern FILE *stderr;

extern void stdio_init();

#endif
