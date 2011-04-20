#include <stdio.h>
#include <stdlib.h>
#include <stream.h>
#include <stdarg.h>
#include <string.h>
#include <vga.h>

// http://www.acm.uiuc.edu/webmonkeys/book/c_guide/2.12.html#streams

/*
 * virtual filesystem
 */

typedef struct {
	char name[256];
	unsigned char *data;
	size_t size;
	size_t max;
} file_entry;

file_entry *root[256];

typedef struct {
	file_entry *entry;
	unsigned int pos;
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
	if (f->pos >= f->entry->max) {
		//[TODO] expand file size
		return EOF;
	}
	if (f->pos + 1 > f->entry->size)
		f->entry->size = f->pos + 1;
	f->entry->data[f->pos++] = c;
	return (int) c;
}

int file_entry_seek(stream_s *stream, long pos, int origin)
{
	file_entry_data *f = (file_entry_data *) stream->data;
	f->pos = pos;
	return 0;
}


/*
 * file functions
 */

void clearerr(FILE *file)
{
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
	if (file == NULL)
		return EOF;
	return file->pos;
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
			file->hold = 0;
			return file;
		}
	}
	for (i = 0; i < 256; i++) {
		if (root[i] == NULL) {
			//puts("Creating file...\n");
			
			root[i] = (file_entry *) malloc(sizeof(file_entry));
			strncpy((char *) &(root[i]->name), filename, 256);
			root[i]->data = (unsigned char *) malloc(1024);
			root[i]->size = 0;
			root[i]->max = 1024;
			
			return fopen(filename, mode);
		}
	}
	puts("ERROR: No file space left!");
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
	if (file->stream == NULL)
		return EOF;
		
	file->pos++;
	return file->stream->read(file->stream);
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
	//[TODO]
	return EOF;
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
