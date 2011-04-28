#include <common.h>
#include <stdio.h>
#include <stdlib.h>
#include <stream.h>
#include <stdarg.h>
#include <string.h>
#include <stdint.h>
#include <errno.h>
#include <stream.h>
#include <keyboard.h>
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

int file_entry_get(stream_s *stream)
{
	file_entry_data *f = (file_entry_data *) stream->data;
	if (f->pos >= f->entry->size)
		return EOF;
	return f->entry->data[f->pos++];
}

long int file_entry_avail(stream_s *stream)
{
	file_entry_data *f = (file_entry_data *) stream->data;
	return f->entry->size - f->pos;
}

int file_entry_put(stream_s *stream, unsigned char c)
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

static FILE* create_file(
	stream_s *stream		// stream implementation
	)
{
	FILE *f = malloc(sizeof(FILE));
	f->stream = stream;
	f->pos = 0;
	f->flags = 0;
	f->istemp = 0;
	f->buffer = NULL;
	f->bsize = 0;
	f->eof = 0;
	f->hold = EOF;
	return f;	
}

/*
 * file functions
 */

void clearerr(FILE *file)
{
	file->eof = 0;
	file->stream->ferr = 0;
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
	return file->stream->ferr;
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
			stream->get = file_entry_get;
			stream->put = file_entry_put;
			stream->seek = file_entry_seek;
			stream->data = data;
			
			return create_file(stream);
		}
	}
	
	if (strcmp(mode, "r") != 0) {
		for (i = 0; i < 256; i++) {
			if (root[i] == NULL) {
				//puts("Creating file...\n");
				
				root[i] = (file_entry *) malloc(sizeof(file_entry));
				strncpy((char *) &(root[i]->name), filename, 256);
				root[i]->data = NULL;
				root[i]->size = 0;
				root[i]->max = 0;
				
				return fopen(filename, mode);
			}
		}
		puts("ERROR: Exceeded max of 256 files!");
	}
	return NULL;
}

size_t fread(void *ptr, size_t size, size_t nmemb, FILE *file)
{	
	unsigned char *p = (unsigned char *) ptr;
	size_t i, j;
	int b;
	for (i = 0; i < nmemb; i++) {
		for (j = 0; j < size; j++) {
			if ((b = fgetc(file)) == EOF) break;
			p[i*size + j] = (unsigned char) b;
		}
		if (b == EOF) break;
	}
	return i;
}

FILE *freopen(const char *filename, const char *mode, FILE *stream)
{
	UNUSED(filename); UNUSED(mode); UNUSED(stream);
	puts("freopen() called\n");
	return NULL;
}

int fseek(FILE *file, long int offset, int whence)
{
	puts("fseek() called\n");
	file->stream->seek(file->stream, offset, whence);
	return -1;
}

long int ftell(FILE *file)
{
	return file->pos;
}

size_t fwrite(const void *ptr, size_t size, size_t nmemb, FILE *file)
{
	//[TODO] does this defy the size component of fwrite?
	unsigned char *p = (unsigned char *) ptr;
	unsigned int i;
	size_t j;
	int b;
	for (i = 0; i < nmemb; i++) {
		for (j = 0; j < size; j++) {
			if ((b = fputc(p[i*size + j], file)) == EOF) break;
		}
		if (b == EOF) break;
	}
	return i;
}

int remove(const char *filename)
{
	UNUSED(filename);
	puts("remove() called\n");
	return -1;
}

int rename(const char *old_filename, const char *new_filename)
{
	UNUSED(old_filename); UNUSED(new_filename);
	puts("rename() called\n");
	return -1;
}

void rewind(FILE *file)
{
	UNUSED(file);
	puts("rewind() called\n");
}

void setbuf(FILE *stream, char *buffer)
{
	UNUSED(stream); UNUSED(buffer);
	puts("setbuf() called\n");
}

int setvbuf(FILE *stream, char *buffer, int mode, size_t size)
{
	UNUSED(stream); UNUSED(buffer); UNUSED(mode); UNUSED(size);
	puts("setvbuf() called\n");
	return -1;
}

FILE *tmpfile(void)
{
	puts("tmpfile() called\n");
	return NULL;
}

char *tmpnam(char *str)
{
	UNUSED(str);
	puts("tmpnam() called\n");
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
	puts("fscanf() called\n");
	return -1;
}

int scanf(const char *format, ...)
{
	UNUSED(format);
	puts("scanf() called\n");
	return -1;
}

int sscanf(const char *str, const char *format, ...)
{
	UNUSED(str); UNUSED(format);
	puts("sscanf() called\n");
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
	int a = file->stream->get(file->stream);
	if (a == EOF)
		file->eof = 1;
	return a;
}

char *fgets(char *str, int n, FILE *file)
{
	if (file->stream == NULL)
		return NULL;
		
	file->pos++;
	int c = EOF, i = 0;
	while (i < n && c != '\n' && (c = fgetc(file)) != EOF) {
		str[i++] = c;
	}
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
	//if (errno) {
		if (s)
			fprintf(stderr, "%s: %s\n", s, strerror(errno));
		else
			fprintf(stderr, "%s\n", strerror(errno));
	//}
}

/*
 * standard streams
 */

FILE *stdout;
FILE *stdin;
FILE *stderr;

void stdio_init()
{
	stdout = create_file(vgaout);
	stdin = create_file(keyboardin);
	stderr = stdout; // [TODO] change this
}