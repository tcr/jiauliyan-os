#ifndef __STREAM_H
#define __STREAM_H

#ifndef NULL
#define NULL    0
#endif

#define EOF -1

#define size_t unsigned int

typedef struct __stream {
	int (*read)(struct __stream *stream); // reads a character
	int (*write)(struct __stream *stream, unsigned char s); // returns EOF on error, otherwise (int) s
	int (*seek)(struct __stream *stream, long pos, int whence); // seeks to position
	void *data; // data object
} stream_s;

extern int stream_no_read(stream_s *stream);
extern int stream_no_write(stream_s *stream, unsigned char s);
extern int stream_no_seek(stream_s *stream, long pos, int whence);

#define stream_putc(S, C) S->write(S, (char) C)
extern size_t stream_puts(stream_s *stream, const char *str);
extern void stream_puti(stream_s *stream, unsigned int i);
extern void stream_puthc(stream_s *stream, unsigned char c);
extern void stream_putp(stream_s *stream, void *p);

#endif
