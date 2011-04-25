#include <common.h>
#include <stdio.h>
#include <stdlib.h>
#include <stream.h>
#include <stdarg.h>
#include <string.h>
#include <stdint.h>
#include <errno.h>

#include <stream.h>
#include <vga.h>
#include <serial.h>

// http://www.acm.uiuc.edu/webmonkeys/book/c_guide/2.12.html#streams

/*
 * virtual filesystem
 */

typedef struct {
	char name[256];
	unsigned char *data;
	long int size;
	long int max;
} file_entry;

file_entry *root[256];

typedef struct {
	file_entry *entry;
	long int pos;
} file_entry_data;

/* file entry data stream */

int file_entry_read(stream_s *stream)
{
	file_entry_data *f = (file_entry_data *) stream->data;
	if (f->pos >= f->entry->size)
		return EOF;
	return f->entry->data[f->pos++];
}

int file_entry_write(stream_s *stream, unsigned char c)
{
	file_entry_data *f = (file_entry_data *) stream->data;
	
	// expand file size
	if (f->pos >= f->entry->max) {
		f->entry->max += 1024;
		f->entry->data = (unsigned char *) realloc(f->entry->data, f->entry->max);
	}

	if (f->pos + 1 > f->entry->size)
		f->entry->size = f->pos + 1;
	f->entry->data[f->pos++] = c;
	return (int) c;
}

int file_entry_seek(stream_s *stream, long pos, int origin)
{
	UNUSED(origin);
	file_entry_data *f = (file_entry_data *) stream->data;
	f->pos = pos;
	return 0;
}

/* file object */

//static FILE* create_file(


/*
 * file functions
 */

void clearerr(FILE *file)
{
	UNUSED(file);
	stream_puts(serialout, "clearerr() called");
	//[TODO]
}

int fclose(FILE *file)
{
	if (file == NULL || file->stream == NULL)
		return EOF;
	file->stream = NULL;
	return 0;
}

int feof(FILE *file)
{
	return file->eof;
}

int ferror(FILE *file)
{
	UNUSED(file);
	return 0;
}

int fflush(FILE *file)
{
	UNUSED(file);
	return -1;
}

int fgetpos(FILE *file, int *pos)
{
	if (file->stream == NULL)
		return EOF;
	*pos = file->pos;
	return 0;
}

FILE *fopen(const char *filename, const char *mode)
{
	int i;
	for (i = 0; i < 256; i++) {
		if (root[i] != NULL && strcmp((char *) &(root[i]->name), filename) == 0) {
			//puts("Opening file...\n");
						
			file_entry_data *data = (file_entry_data *) malloc(sizeof(file_entry_data));
			data->entry = root[i];
			data->pos = 0;
			
			stream_s *stream = (stream_s *) malloc(sizeof(stream_s));
			stream->read = file_entry_read;
			stream->write = file_entry_write;
			stream->seek = file_entry_seek;
			stream->data = data;
			
			FILE *file = (FILE *) malloc(sizeof(FILE));
			file->stream = stream;
			file->pos = 0;
			file->flags = 0;
			file->istemp = 0;
			file->buffer = NULL;
			file->bsize = 0;
			file->eof = 0;
			file->hold = 0;
			return file;
		}
	}
	for (i = 0; i < 256; i++) {
		if (root[i] == NULL) {
			//puts("Creating file...\n");
			
			root[i] = (file_entry *) malloc(sizeof(file_entry));
			strncpy((char *) &(root[i]->name), filename, 256);
			root[i]->data = NULL;
			root[i]->size = 0;
			root[i]->max = 1024;
			
			return fopen(filename, mode);
		}
	}
	puts("ERROR: Exceeded max of 256 files!");
	return NULL;
}

size_t fread(void *ptr, size_t size, size_t nmemb, FILE *file)
{
	unsigned char *p = (unsigned char *) ptr;
	unsigned int i;
	size_t j;
	int a;
	for (i = 0; i < nmemb; i++) {
		for (j = 0; j < size; j++) {
			a = file->stream->read(file->stream);
			if (a == EOF) break;
			p[i*size + j] = (unsigned char) a;
		}
		if (a == EOF) break;
	}
	stream_puti(serialout, size);
	stream_puts(serialout, " - ");
	stream_puti(serialout, nmemb);
	stream_puts(serialout, " fread() called\n");
	return i;
}

FILE *freopen(const char *filename, const char *mode, FILE *stream)
{
	UNUSED(filename); UNUSED(mode); UNUSED(stream);
	stream_puts(serialout, "freopen() called\n");
	return NULL;
}

int fseek(FILE *file, long int offset, int whence)
{
	stream_puts(serialout, "fseek() called\n");
	file->stream->seek(file->stream, offset, whence);
	return -1;
}

long int ftell(FILE *file)
{
	return file->pos;
}

size_t fwrite(const void *ptr, size_t size, size_t nmemb, FILE *file)
{
	UNUSED(ptr); UNUSED(size); UNUSED(nmemb); UNUSED(file);
	stream_puts(serialout, "fwrite() called\n");
	return -1;
}

int remove(const char *filename)
{
	UNUSED(filename);
	stream_puts(serialout, "remove() called\n");
	return -1;
}

int rename(const char *old_filename, const char *new_filename)
{
	UNUSED(old_filename); UNUSED(new_filename);
	stream_puts(serialout, "rename() called\n");
	return -1;
}

void rewind(FILE *file)
{
	UNUSED(file);
	stream_puts(serialout, "rewind() called\n");
}

void setbuf(FILE *stream, char *buffer)
{
	UNUSED(stream); UNUSED(buffer);
	stream_puts(serialout, "setbuf() called\n");
}

int setvbuf(FILE *stream, char *buffer, int mode, size_t size)
{
	UNUSED(stream); UNUSED(buffer); UNUSED(mode); UNUSED(size);
	stream_puts(serialout, "setvbuf() called\n");
	return -1;
}

FILE *tmpfile(void)
{
	stream_puts(serialout, "tmpfile() called\n");
	return NULL;
}

char *tmpnam(char *str)
{
	UNUSED(str);
	stream_puts(serialout, "tmpnam() called\n");
	return NULL;
}

/*
 * formatted I/O
 */
 
int fprintf(FILE *stream, const char *format, ...)
{
	va_list ap;
	int ret;

	va_start(ap, format);
	ret = vfprintf(stream, format, ap);
	va_end(ap);

	return ret;
}

int printf(const char *format, ...)
{
	va_list ap;
	int ret;

	va_start(ap, format);
	ret = vfprintf(stdout, format, ap);
	va_end(ap);

	return ret;
}

int sprintf(char *str, const char *format, ...)
{
	/*
	UNUSED(str); UNUSED(format);
	str[0] = '1';
	str[1] = '0';
	str[2] = '.';
	str[3] = '0';
	str[4] = '\0';
	stream_puts(serialout, format);
	stream_puts(serialout, " sprintf() called\n");
	return 1;
	*/

	va_list ap;
	int ret;

	va_start(ap, format);
	ret = vsprintf(str, format, ap);
	va_end(ap);

	return ret;
}

int vfprintf(FILE *file, const char *format, va_list arg)
{
	if (!file)
		return -1;
	return stream_vformat(file->stream, format, arg);
}

int vprintf(const char *format, va_list arg)
{
	return vfprintf(stdout, format, arg);
}

int vsprintf(char *str, const char *format, va_list arg)
{
	stream_s *bs = bytestream_create(1024);
	int ret = stream_vformat(bs, format, arg);
	if (ret >= 0) {
		stream_putc(bs, '\0');
		unsigned char *bs_str = bytestream_data(bs);
		strncpy(str, (char *) bs_str, (size_t) bytestream_size(bs));
	}
	bytestream_destroy(bs);
	return ret;
}

/*
 * parse I/O
 */
 
int fscanf(FILE *stream, const char *format, ...)
{
	UNUSED(stream); UNUSED(format);
	stream_puts(serialout, "fscanf() called\n");
	return -1;
}

int scanf(const char *format, ...)
{
	UNUSED(format);
	stream_puts(serialout, "scanf() called\n");
	return -1;
}

int sscanf(const char *str, const char *format, ...)
{
	UNUSED(str); UNUSED(format);
	stream_puts(serialout, "sscanf() called\n");
	return -1;
}

/*
 * character I/O
 */

int fgetc(FILE *file)
{
	if (file->stream == NULL)
		return EOF;
	if (file->hold != EOF) {
		file->pos++;
		int hold = file->hold;
		file->hold = EOF;
		return hold;
	}
		
	file->pos++;
	int a = file->stream->read(file->stream);
	if (a == EOF)
		file->eof = 1;
	return a;
}

char *fgets(char *str, int n, FILE *file)
{
	if (file->stream == NULL)
		return NULL;
		
	file->pos++;
	int c = '\0', i = 0;
	while (i < n && c != '\n' && (c = fgetc(file)) != EOF)
		str[i++] = c;
	str[i] = '\0';
	return str;
}

int fputc(int c, FILE *file)
{
	if (file->stream == NULL)
		return EOF;
		
	file->pos++;
	return stream_putc(file->stream, (char) c);
}

int fputs(const char *string, FILE *file)
{
	if (file->stream == NULL)
		return EOF;
		
	file->pos += strlen(string) + 1;
	return stream_puts(file->stream, string);
}

int ungetc(int c, FILE *file)
{
	if (file->hold != EOF || c == EOF)
		return EOF;
	file->pos--;
	file->hold = c;
	return c;
}

/*
 * error functions
 */
 
void perror(const char *s)
{
	if (errno) {
		if (s)
			fprintf(stderr, "%s: %s\n", s, strerror(errno));
		else
			fprintf(stderr, "%s\n", strerror(errno));
	}
}

/*
 * standard streams
 */

FILE fstdout = { NULL, 0L, 0, 0, NULL, 0, 0, '\0' };
FILE *stdout = &fstdout;
FILE *stdin;
FILE *stderr = &fstdout; // [TODO] change this

void stdio_init()
{
	fstdout.stream = vgastream;
}
