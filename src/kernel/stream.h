/*
 * Jiauliyan OS - Released under the MIT License
 * Copyright (C) 2011 Paul Booth, Jialiya Huang, Tim Ryan
 * https://github.com/timcameronryan/jiauliyan
 * 
 * Based on work by Nick Johnson's Rhombus OS, under the OpenBSD license
 * Copyright (C) 2009, 2010 Nick Johnson <nickbjohnson4224 at gmail.com>
 * https://github.com/nickbjohnson4224/rhombus/
 */
 
#ifndef __STREAM_H
#define __STREAM_H

#include <stddef.h>
#include <stdarg.h>

#define EOF -1

#define SEEK_SET	0
#define SEEK_CUR	1
#define SEEK_END	2

#define _IOFBF          0x000
#define _IOLBF          0x001
#define _IONBF          0x002

#define BUFSIZ 1024

typedef struct __stream {
	int (*get)(struct __stream *stream); // reads a character
	int (*put)(struct __stream *stream, unsigned char s); // returns EOF on error, otherwise (int) s
	long int (*avail)(struct __stream *stream); // minimum # of bytes guaranteed to be available
	int (*seek)(struct __stream *stream, long pos, int whence); // seeks to position
	void *data; // data object
	int ferr;
} stream_s;

extern stream_s *stream_create(
	int (*get)(struct __stream *stream),
	int (*put)(struct __stream *stream, unsigned char s),
	long int (*avail)(struct __stream *stream),
	int (*seek)(struct __stream *stream, long pos, int whence),
	void *data);

extern int stream_no_get(stream_s *stream);
extern int stream_no_put(stream_s *stream, unsigned char s);
extern long int stream_no_avail(stream_s *stream);
extern int stream_no_seek(stream_s *stream, long pos, int whence);

/*
 * formatting
 */

#define FLAG_SIGN		0x01	// Always show sign
#define FLAG_LEFT		0x02	// Left align
#define FLAG_ALT		0x04	// Alternate form
#define FLAG_ZERO		0x08	// Zero-pad
#define FLAG_UPPER		0x10	// Upper case
#define FLAG_OCTAL		0x20	// Octal output
#define FLAG_HEX		0x40	// Hexadecimal output
#define FLAG_EXP		0x80	// Use scientific notation
#define FLAG_MEXP		0x100	// Maybe use scientific notation

#define LENGTH_BYTE		0		// char
#define LENGTH_SHORT	1		// short int
#define LENGTH_LONG		3		// int or double
#define LENGTH_LLONG	4		// long long int or long double

extern size_t stream_putbyte(stream_s *stream, unsigned char b);
extern size_t stream_putchar(stream_s *stream, char c);
extern size_t stream_putstring(stream_s *stream, const char *str);
extern size_t stream_putint(stream_s *stream, int i, int flags);
extern size_t stream_putuint(stream_s *stream, unsigned int i, int flags);
extern size_t stream_putdouble(stream_s *stream, long double value, int precision, int flags);
extern size_t stream_puthc(stream_s *stream, unsigned char c);
#define stream_putpointer(S, P) stream_format(S, "%#p", P)

extern size_t stream_format(stream_s *stream, const char *format, ...);
extern size_t stream_vformat(stream_s *stream, const char *format, va_list ap);

/* bytestream */
extern stream_s *bytestream_create(long int capacity);
extern void bytestream_destroy(stream_s *stream);
extern long int bytestream_size(stream_s *stream);
extern unsigned char *bytestream_data(stream_s *stream);

#endif
