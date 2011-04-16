#ifndef __STREAM_H
#define __STREAM_H

#ifndef NULL
#define NULL    0
#endif

#define EOF -1

typedef struct __stream {
	char (*read)(struct __stream *stream); // reads a character
	int (*write)(struct __stream *stream, unsigned char s); // returns EOF on error, otherwise (int) s
	int (*seek)(struct __stream *stream, long pos, int origin); // seeks to position
	void *data; // data object
} stream_s;

#define stream_putc(S, C) S->write(S, (char) C)
extern int stream_puts(stream_s *stream, const char *str);
extern void stream_puti(stream_s *stream, unsigned int i);
extern void stream_puthc(stream_s *stream, unsigned char c);
extern void stream_putp(stream_s *stream, void *p);

#endif
