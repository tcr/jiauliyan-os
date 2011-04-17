#include <stream.h>
#include <string.h>

/*
 * stream impl dummies
 */

int stream_no_read(stream_s *stream)
{
	(void) stream;
	return EOF;
}

int stream_no_write(stream_s *stream, unsigned char s)
{
	(void) stream; (void) s;
	return EOF;
}

int stream_no_seek(stream_s *stream, long pos, int whence)
{
	(void) stream; (void) pos; (void) whence;
	return -1;
}

/*
 * text output
 */

/* put a string */
size_t stream_puts(stream_s *stream, const char *text)
{
	int i;
	for (i = 0; i < strlen(text); i++)
	{
		stream_putc(stream, text[i]); 
	}
	return 1;
}

/* put a decimal integer */
void stream_puti(stream_s *stream, unsigned int a)
{
	do {
		/* print highest digit */
		unsigned int b = a, d = 1;
		while ((b / 10) > 0) {
			d *= 10;
			b /= 10;
		}
		stream_putc(stream, 0x30 + b);
	
		/* print rest */
		while (a >= d)
			a -= d;
	} while (a > 0);
}

/* put a hex char */
void stream_puthc(stream_s *stream, unsigned char h)
{
	int b = h & 0xF, a = (h >> 4) & 0xF;
	stream_putc(stream, a > 0x9 ? 0x61 - 0x0A + a : 0x30 + a);
	stream_putc(stream, b > 0x9 ? 0x61 - 0x0A + b : 0x30 + b);
}

/* put a pointer */
void stream_putp(stream_s *stream, void *p)
{
	int i, siz = sizeof(void *);
	stream_puts(stream, "0x");
	for (i = siz - 1; i >= 0; i--) {
		stream_puthc(stream, (((int) p) >> (i*8)) & 0xFF);
	}
}
