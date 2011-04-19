#include <stdio.h>
#include <stream.h>
#include <stdarg.h>
#include <vga.h>

// http://www.acm.uiuc.edu/webmonkeys/book/c_guide/2.12.html#streams

/*
 * file functions
 */

void clearerr(FILE *file)
{
	//[TODO]
}

int fclose(FILE *file)
{
	return -1;
}

int feof(FILE *file)
{
	return -1;
}

int ferror(FILE *file)
{
	return -1;
}

int fflush(FILE *file)
{
	return -1;
}

int fgetpos(FILE *file, int *pos)
{
	return -1;
}

FILE *fopen(const char *filename, const char *mode)
{
	return NULL;
}

size_t fread(void *ptr, size_t size, size_t nmemb, FILE *stream)
{
	return -1;
}

FILE *freopen(const char *filename, const char *mode, FILE *stream)
{
	return NULL;
}

int fseek(FILE *file, long int offset, int whence)
{
	return -1;
}

long int ftell(FILE *file)
{
	return -1;
}

size_t fwrite(const void *ptr, size_t size, size_t nmemb, FILE *file)
{
	return -1;
}

int remove(const char *filename)
{
	return -1;
}

int rename(const char *old_filename, const char *new_filename)
{
	return -1;
}

void rewind(FILE *file)
{
	return -1;
}

void setbuf(FILE *stream, char *buffer)
{
	return -1;
}

int setvbuf(FILE *stream, char *buffer, int mode, size_t size)
{
	return -1;
}

FILE *tmpfile(void)
{
	return NULL;
}

char *tmpnam(char *str)
{
	return NULL;
}

/*
 * formatted I/O
 */
 
int fprintf(FILE *stream, const char *format, ...)
{
	return -1;
}

int printf(const char *format, ...)
{
	return -1;
}

int sprintf(char *str, const char *format, ...)
{
	return -1;
}

int vfprintf(FILE *stream, const char *format, va_list arg)
{
	return -1;
}

int vprintf(const char *format, va_list arg)
{
	return -1;
}

int vsprintf(char *str, const char *format, va_list arg)
{
	return -1;
}

/*
 * parse I/O
 */
 
int fscanf(FILE *stream, const char *format, ...)
{
	return -1;
}

int scanf(const char *format, ...)
{
	return -1;
}

int sscanf(const char *str, const char *format, ...)
{
	return -1;
}

/*
 * character I/O
 */

int fgetc(FILE *file)
{
	return file->stream->read(file->stream);
}

char *fgets(char *str, int n, FILE *file)
{
	int c = '\0', i = 0;
	while (i < n && c != '\n' && (c = fgetc(file)) != EOF)
		str[i++] = c;
	str[i] = '\0';
	return str;
}

int fputc(int c, FILE *file)
{
	return stream_putc(file->stream, (char) c);
}

int fputs(const char *string, FILE *file)
{
	return stream_puts(file->stream, string);
}

/*
 * standard streams
 */

FILE fstdout = { NULL, 0L, 0, 0, NULL, 0, '\0' };
FILE *stdout = &fstdout;
FILE *stdin;
FILE *stderr;

void stdio_init()
{
	fstdout.stream = vgastream;
}
