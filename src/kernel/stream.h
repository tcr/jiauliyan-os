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

extern int stream_format(stream_s *stream, const char *format, ...);
extern int stream_vformat(stream_s *stream, const char *format, va_list ap);

#define stream_putb(S, C) S->put(S, (unsigned char) C)
#define stream_putc(S, C) S->put(S, (char) C)
extern size_t stream_puts(stream_s *stream, const char *str);
extern void stream_puti(stream_s *stream, unsigned int i);
extern void stream_puthc(stream_s *stream, unsigned char c);
extern void stream_putp(stream_s *stream, void *p);

/* bytestream */
extern stream_s *bytestream_create(long int capacity);
extern void bytestream_destroy(stream_s *stream);
extern long int bytestream_size(stream_s *stream);
extern unsigned char *bytestream_data(stream_s *stream);

#endif
