#include <stdio.h>
#include <stream.h>
#include <vga.h>

int fputc(int c, FILE *file)
{
	return stream_putc(file->stream, (char) c);
}

int fputs(const char *string, FILE *file)
{
	return stream_puts(file->stream, string);
}

/* stdio implementation */

FILE fstdout = { NULL, 0L, 0, 0, 0L, 0, '\0' };

FILE *stdout = &fstdout;
FILE *stdin;
FILE *stderr;

void stdio_init()
{
	fstdout.stream = vgastream;
}
